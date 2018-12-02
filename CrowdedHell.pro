#-------------------------------------------------
#
# Project created by QtCreator 2018-12-02T01:09:40
#
#-------------------------------------------------

QT       += core gui opengl multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrowdedHell
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

INCLUDEPATH += \
        $$PWD/FMOD/inc

LIBS += \
        -L$$PWD/FMOD/lib -lfmod -lfmodL

SOURCES += \
        main.cpp \
        crowdedhellgui.cpp \
    display/avoidancedisplaywidget.cpp

HEADERS += \
        crowdedhellgui.h \
    display/avoidancedisplaywidget.h

FORMS += \
        crowdedhellgui.ui

TRANSLATIONS += \
        Trans_zh_cn.ts \
        Trans_zh_tw.ts \
        Trans_jp.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    translation.qrc
