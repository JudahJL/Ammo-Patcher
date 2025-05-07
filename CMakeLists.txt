cmake_minimum_required(VERSION 3.29)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")
########################################################################################################################
## Define project
########################################################################################################################
project(
        Ammo_Patcher
        VERSION 2.0.0.1
        DESCRIPTION "A simple SKSE plugin built with CommonLibSSE-NG to patch AMMO"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)

include(GNUInstallDirs)
include(FetchContent)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

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

list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/src/PCH.cpp)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${headers}
        ${sources})

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

find_package(RapidJSON CONFIG REQUIRED)
find_package(CommonLibSSE CONFIG REQUIRED)
FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG main
)

FetchContent_MakeAvailable(ctre)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources} ${misc} AUTHOR ${PROJECT_AUTHOR})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        rapidjson
        ctre)

target_precompile_headers(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/PCH.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/PCH.cpp)

install(TARGETS
        ${PROJECT_NAME}
        DESTINATION
        "${CMAKE_INSTALL_LIBDIR}")

if (MSVC)
    add_compile_options(/W4 /w14820)
endif ()

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

########################################################################################################################
## Automatic plugin deployment
########################################################################################################################
# Automatic deployment to FOMOD directory.
set(DISTRIBUTION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib/Distribution/")
set(CONTRIB_IMAGES_DIR "${DISTRIBUTION_DIR}Images/")
set(CONTRIB_PROJECT_DIR "${DISTRIBUTION_DIR}/Plugin - ${CMAKE_BUILD_TYPE}/")
set(CONTRIB_COMMON_DIR "${DISTRIBUTION_DIR}/Common/")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTRIB_PROJECT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTRIB_IMAGES_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CONTRIB_COMMON_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Images/AmmoPatcherLogo.png" "${CONTRIB_IMAGES_DIR}")

file(
        GLOB_RECURSE
        json_files
        "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/**.json"
)

foreach (json_file ${json_files})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${json_file} "${CONTRIB_COMMON_DIR}")
endforeach ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> "${CONTRIB_PROJECT_DIR}")
endif ()

foreach (DEPLOY_TARGET $ENV{SkyrimPluginTargets})
    set(DEPLOY_TARGET_COMMON_DIR "${DEPLOY_TARGET}/${PROJECT_NAME} - ${CMAKE_BUILD_TYPE}/SKSE/Plugins/")
    set(DEPLOY_TARGET_CUSTOM_COMMON_DIR "${DEPLOY_TARGET}/Common for ${PROJECT_NAME}/SKSE/Plugins/")
    set(DEPLOY_TARGET_PRESETS_DIR "${DEPLOY_TARGET_COMMON_DIR}Ammo Patcher/Presets/")
    message(STATUS "Adding deployment target ${DEPLOY_TARGET}.")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_PRESETS_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEPLOY_TARGET_CUSTOM_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/Ammo_Patcher.json" "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/custom/Ammo_Patcher.json" "${DEPLOY_TARGET_CUSTOM_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/APConfig_schema.json" "${DEPLOY_TARGET_COMMON_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/Action Based Projectiles.json" "${DEPLOY_TARGET_PRESETS_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/Default.json" "${DEPLOY_TARGET_PRESETS_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/json/main config/Vanilla.json" "${DEPLOY_TARGET_PRESETS_DIR}"
    )

    if (${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DEPLOY_TARGET_COMMON_DIR}")
    endif ()
endforeach ()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/skse-fomod.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compress-artifact.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/automatic-github-workflow-setup.cmake)