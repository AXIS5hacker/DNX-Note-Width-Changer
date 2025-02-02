cmake_minimum_required (VERSION 3.5)

project (Chartstore LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#set include directories
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../include")

#set static compile flags in MSVC
if (MSVC)
    set(CMAKE_BUILD_TYPE "Release")
    #static MFC
    set(CMAKE_MFC_FLAG 1)
    #compile settings: /MT
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif(MSVC)

set(PROJECT_SOURCES
    chart_store.cpp
)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})