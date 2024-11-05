cmake_minimum_required(VERSION 3.29)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")
########################################################################################################################
## Define project
########################################################################################################################

set(CURRENT_PROJECT_MAJOR_VER 1)
set(CURRENT_PROJECT_MINOR_VER 3)
set(CURRENT_PROJECT_PATCH_VER 2)

message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")

project(
        Ammo_Patcher
        VERSION "${CURRENT_PROJECT_MAJOR_VER}.${CURRENT_PROJECT_MINOR_VER}.${CURRENT_PROJECT_PATCH_VER}"
        DESCRIPTION "A simple SKSE plugin built with CommonLibSSE-NG to patch AMMO"
        LANGUAGES CXX)

message(STATUS "Version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

include(GNUInstallDirs)

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "in-source builds are not allowed")
endif ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif ()

#if (POLICY CMP0141)
#    cmake_policy(SET CMP0141 NEW)
#    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif ()
#
#message(STATUS "CMAKE_MSVC_DEBUG_INFORMATION_FORMAT is set to ${CMAKE_MSVC_DEBUG_INFORMATION_FORMAT}")

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/Plugin.h
        @ONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/version.rc
        @ONLY
)

# Add headers
file(
        GLOB_RECURSE
        headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)
list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/include/PCH.h)

# Add sources
file(
        GLOB_RECURSE
        sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

# Exclude specific subdirectory from sources
foreach(file_path ${sources})
    if(file_path MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/src/SkyPatcher/.*")
        list(REMOVE_ITEM sources ${file_path})
        message(STATUS "Removed ${file_path}")
    endif()
endforeach()

# Exclude specific subdirectory from headers
foreach(file_path ${headers})
    if(file_path MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/include/SkyPatcher/.*")
        list(REMOVE_ITEM sources ${file_path})
        message(STATUS "Removed ${file_path}")
    endif()
endforeach()

set(tests)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${headers}
        ${sources}
        ${tests})

#########################################################################################################################
### Build options
#########################################################################################################################
message("Options:")
option(BUILD_TESTS "Build unit tests." OFF)
message("\tTests: ${BUILD_TESTS}")

########################################################################################################################
## Configure target DLL
########################################################################################################################
set(PROJECT_AUTHOR "NoName365")

if(DEFINED ENV{GITHUB_ENV})
    # If running in a GitHub Actions environment, always use the submodule directory.
    set(CommonLibSSE "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE-NG/")
    message(STATUS "GitHub Actions detected. Using CommonLibSSE submodule directory.")
elseif(DEFINED ENV{CommonLibSSEPath})
    # If CommonLibSSEPath is set, use the provided environment variable.
    set(CommonLibSSE $ENV{CommonLibSSEPath})
    message(STATUS "CommonLibSSEPath environment variable found. Using: ${CommonLibSSE}")
else()
    # If not in GitHub Actions and CommonLibSSEPath is not set, fallback to the submodule directory.
    set(CommonLibSSE "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE-NG/")
    message(WARNING "CommonLibSSEPath not defined. Using submodule directory: ${CommonLibSSE}")
    message(WARNING "Ensure that the CommonLibSSE-NG submodule is initialized.")
endif()


if(NOT DEFINED CommonLibSSE)
        message(FATAL_ERROR "Variable CommonLibSSE is not Defined.Make sure to define it")
endif()

message(STATUS "CommonLibSSE Path: ${CommonLibSSE}")

add_subdirectory(${CommonLibSSE} CommonLibSSE-NG)
include(${CommonLibSSE}/cmake/CommonLibSSE.cmake)

find_package(nlohmann_json 3.11.3 REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources} ${misc} AUTHOR ${PROJECT_AUTHOR})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>
        nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        include/PCH.h)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")


########################################################################################################################
## Automatic plugin deployment
########################################################################################################################
# Automatic deployment to FOMOD directory.
set(DISTRIBUTION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Distribution/")
set(CONTRIB_PROJECT_DIR "${DISTRIBUTION_DIR}/Plugin - ${CURRENT_BUILD_TYPE}/")
set(CONTRIB_CONFIGURATION_GLOBAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Configuration/Global/")
set(CONTRIB_CONFIGURATION_MY_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Configuration/My Config/")
set(CONTRIB_COMMON_DIR "${DISTRIBUTION_DIR}/Common/")

file(GLOB_RECURSE GLOBAL_JSON_FILES "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/**.json")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTRIB_PROJECT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")

foreach(json_file ${GLOBAL_JSON_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${json_file}" "${CONTRIB_COMMON_DIR}")
endforeach()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")
endif()

file(GLOB_RECURSE OUTPUT_DLLS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.dll")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    file(GLOB_RECURSE OUTPUT_PDBS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.pdb")
endif()

file(GLOB_RECURSE OUTPUT_JSONS "${CONTRIB_COMMON_DIR}/**.json")

set_property(TARGET ${PROJECT_NAME}
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_DLLS}"
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_JSONS}")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set_property(TARGET ${PROJECT_NAME}
            APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_PDBS}")
endif()

# Automatic deployment to Mod Organizer 2 mod directory.
foreach (DEPLOY_TARGET $ENV{SkyrimPluginTargets})    #set your "SkyrimPluginTargets" environment variable to your MO2's Mods Folder
        set(DEPLOY_TARGET_COMMON_DIR "${DEPLOY_TARGET}/${PROJECT_NAME} - ${CMAKE_BUILD_TYPE}/SKSE/Plugins/")              

        message("Adding deployment target ${DEPLOY_TARGET}.")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"     
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/ExclusionHint.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/MainHint.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET_COMMON_DIR}/"          
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/Action Based Projectiles.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/Vanilla.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_GLOBAL_DIR}/Default.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET}/Common For ${PROJECT_NAME}/SKSE/Plugins/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_MY_CONFIG_DIR}/Ammo_Patcher_Default.json" "${DEPLOY_TARGET}/Common For ${PROJECT_NAME}/SKSE/Plugins/Ammo Patcher/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_MY_CONFIG_DIR}/test.json" "${DEPLOY_TARGET}/Common For ${PROJECT_NAME}/SKSE/Plugins/Ammo Patcher/Presets/")
        if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET_COMMON_DIR}/")
        endif()
endforeach()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/skse-fomod.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compress-artifact.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/automatic-github-workflow-setup.cmake)
