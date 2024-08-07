cmake_minimum_required(VERSION 3.29)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")
########################################################################################################################
## Define project
########################################################################################################################

set(CURRENT_PROJECT_MAJOR_VER 1)
set(CURRENT_PROJECT_MINOR_VER 2)
set(CURRENT_PROJECT_PATCH_VER 0)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CURRENT_BUILD_TYPE "Debug")
    set(CURRENT_PROJECT_BUILD_VER 2)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CURRENT_BUILD_TYPE "Release")
    set(CURRENT_PROJECT_BUILD_VER 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CURRENT_BUILD_TYPE "RelWithDebInfo")
    set(CURRENT_PROJECT_BUILD_VER 4)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CURRENT_BUILD_TYPE "MinSizeRel")
    set(CURRENT_PROJECT_BUILD_VER 8)
endif()

message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")

project(
        Ammo_Patcher
        VERSION "${CURRENT_PROJECT_MAJOR_VER}.${CURRENT_PROJECT_MINOR_VER}.${CURRENT_PROJECT_PATCH_VER}.${CURRENT_PROJECT_BUILD_VER}"
        VERSION "${CURRENT_PROJECT_MAJOR_VER}.${CURRENT_PROJECT_MINOR_VER}.${CURRENT_PROJECT_PATCH_VER}.${CURRENT_PROJECT_BUILD_VER}"
        DESCRIPTION "A simple SKSE plugin built with CommonLibSSE to patch AMMO"
        LANGUAGES CXX)

message(STATUS "Version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

include(GNUInstallDirs)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        cmake_policy(SET CMP0135 NEW)
endif()

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

set(tests )

set(misc
        ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
        ${CMAKE_CURRENT_SOURCE_DIR}/CMakePresets.json
        ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-configuration.json
        ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json
        ${CMAKE_CURRENT_SOURCE_DIR}/.clang-format)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${headers}
        ${sources}
        ${tests}
        ${misc})

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

# add_subdirectory($ENV{CommonLibSSEPath} CommonLibSSE-NG)
# include($ENV{CommonLibSSEPath}/cmake/CommonLibSSE.cmake)
find_package(CommonLibSSE CONFIG REQUIRED)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

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
set(CONTRIB_CONFIGURATION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Configuration/")
set(CONTRIB_COMMON_DIR "${DISTRIBUTION_DIR}/Common/")

file(GLOB_RECURSE JSON_FILES "${CONTRIB_CONFIGURATION_DIR}/*.json")
list(REMOVE_ITEM JSON_FILES "${CONTRIB_CONFIGURATION_DIR}/Test.json")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTRIB_PROJECT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")

foreach(json_file ${JSON_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${json_file}" "${CONTRIB_COMMON_DIR}"
    )
endforeach()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")
endif()
file(GLOB_RECURSE OUTPUT_DLLS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.dll")
file(GLOB_RECURSE OUTPUT_DLLS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.dll")
if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    file(GLOB_RECURSE OUTPUT_PDBS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.pdb")
    file(GLOB_RECURSE OUTPUT_PDBS "${CONTRIB_PROJECT_DIR}/${PROJECT_NAME}.pdb")
endif()
file(GLOB_RECURSE OUTPUT_JSONS "${CONTRIB_COMMON_DIR}/**.json")
file(GLOB_RECURSE OUTPUT_JSONS "${CONTRIB_COMMON_DIR}/**.json")
set_property(TARGET ${PROJECT_NAME}
        APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_DLLS}"
		APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_JSONS}")
if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    set_property(TARGET ${PROJECT_NAME}
            APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${OUTPUT_PDBS}")
endif()

# Automatic deployment to Mod Organizer 2 mod directory.
foreach (DEPLOY_TARGET $ENV{SkyrimPluginTargets})    #set your "SkyrimPluginTargets" environment variable to your MO2's Mods Folder
        set(DEPLOY_TARGET_COMMON_DIR "${DEPLOY_TARGET}/${PROJECT_NAME} - ${CMAKE_BUILD_TYPE}/SKSE/Plugins/")        

        set(DEPLOY_TARGET_COMMON_DIR "${DEPLOY_TARGET}/${PROJECT_NAME} - ${CMAKE_BUILD_TYPE}/SKSE/Plugins/")        

        message("Adding deployment target ${DEPLOY_TARGET}.")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"     
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/ExclusionHint.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/MainHint.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Hints/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET_COMMON_DIR}/"          
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/Action Based Projectiles.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/Vanilla.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/Default.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/Test.json" "${DEPLOY_TARGET_COMMON_DIR}/Ammo Patcher/Presets/"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET}/Common For ${PROJECT_NAME}/SKSE/Plugins/Ammo Patcher/"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CONTRIB_CONFIGURATION_DIR}/My Config/Ammo_Patcher_Default.json" "${DEPLOY_TARGET}/Common For ${PROJECT_NAME}/SKSE/Plugins/Ammo Patcher/")
        if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET_COMMON_DIR}/")
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET_COMMON_DIR}/")
        endif()
endforeach()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/skse-fomod.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compress-artifact.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/automatic-github-workflow-setup.cmake)
