cmake_minimum_required (VERSION 3.5)

project (Dynamix_chart_width_control LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Release")

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
    chart_store.h
    main.cpp
    width.cpp
    width_change.h
)

if(UNIX)
message("Unix-based systems, skip.")
elseif(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_executable(Dynamix_chart_width_control_x64
        ${PROJECT_SOURCES}
        )
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        add_executable(Dynamix_chart_width_control_x86
        ${PROJECT_SOURCES}
        )
    else()
        add_executable(Dynamix_chart_width_control
        ${PROJECT_SOURCES}
        )
    endif()
endif()