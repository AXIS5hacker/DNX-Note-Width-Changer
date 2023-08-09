cmake_minimum_required(VERSION 3.5)

project(dnx_width_change LANGUAGES CXX)

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


set(PROJECT_SOURCES
    chart_store.cpp
    chart_store.h
    defs.h
    DNX_widthGUI_en_en.ts
    DNX_widthGUI_zh_CN.ts
    main.cpp
    maingui.cpp
    maingui.h
    maingui.ui
    mainGUI.qrc
    width_change.cpp
    resources.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dnx_width_change
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(dnx_width_change SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(dnx_width_change
            ${PROJECT_SOURCES}
        )
    endif()
endif()

qt_add_translations(dnx_width_change TS_FILES DNX_widthGUI_en_en.ts DNX_widthGUI_zh_CN.ts)

target_link_libraries(dnx_width_change PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PRIVATE Qt${QT_VERSION_MAJOR}::Core PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
