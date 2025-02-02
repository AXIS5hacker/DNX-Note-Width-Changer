cmake_minimum_required(VERSION 3.5)

project(Dynamix_note_width_changer_GUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

#set static compile flags in MSVC
if (MSVC)
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
    
    DNX_widthGUI_en_en.ts
    DNX_widthGUI_zh_CN.ts
    main.cpp
    maingui.cpp
    maingui.h
    maingui.ui
    maingui.qrc
    width_change.cpp
    resources.rc
    qdefs.h
    hintdlg.cpp
    hintdlg.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Dynamix_note_width_changer_GUI
    #WIN32 disables the console
        WIN32
        ${PROJECT_SOURCES}
    )
    add_dependencies(Dynamix_note_width_changer_GUI Chartstore)
else()
    if(ANDROID)
        add_library(Dynamix_note_width_changer_GUI SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Dynamix_note_width_changer_GUI
        #WIN32 disables the console
            WIN32
            ${PROJECT_SOURCES}
        )
        add_dependencies(Dynamix_note_width_changer_GUI Chartstore)
    endif()
endif()

qt_add_translations(Dynamix_note_width_changer_GUI TS_FILES DNX_widthGUI_en_en.ts DNX_widthGUI_zh_CN.ts)


target_link_libraries(Dynamix_note_width_changer_GUI PRIVATE Chartstore)
target_link_libraries(Dynamix_note_width_changer_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PRIVATE Qt${QT_VERSION_MAJOR}::Core PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

