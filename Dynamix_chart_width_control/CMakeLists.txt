cmake_minimum_required (VERSION 3.5)

project (Dynamix_chart_width_control LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

#if this directory is set as the top level
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    message("Building console program only.")
    add_subdirectory("../Chartstore" ${CMAKE_BINARY_DIR}/lib)
    include_directories ("../include")
endif()

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
    main.cpp
    width.cpp
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_executable(${PROJECT_NAME}_x64
    ${PROJECT_SOURCES}
    )
    add_dependencies(${PROJECT_NAME}_x64 Chartstore)
    target_link_libraries(${PROJECT_NAME}_x64 PRIVATE Chartstore)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    add_executable(${PROJECT_NAME}_x86
    ${PROJECT_SOURCES}
    )
    add_dependencies(${PROJECT_NAME}_x86 Chartstore)
    target_link_libraries(${PROJECT_NAME}_x86 PRIVATE Chartstore)
else()
    add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    )
    add_dependencies(${PROJECT_NAME} Chartstore)
    target_link_libraries(${PROJECT_NAME} PRIVATE Chartstore)
endif()
