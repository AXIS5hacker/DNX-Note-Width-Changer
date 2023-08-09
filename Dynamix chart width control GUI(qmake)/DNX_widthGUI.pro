QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
RC_ICONS += "DNX icon.ico"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chart_store.cpp \
    main.cpp \
    maingui.cpp \
    width_change.cpp

HEADERS += \
    chart_store.h \
    defs.h \
    maingui.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mainGUI.qrc

FORMS += \
    maingui.ui

DISTFILES += \
    DNX_widthGUI_en_en.ts \
    DNX_widthGUI_zh_CN.ts
