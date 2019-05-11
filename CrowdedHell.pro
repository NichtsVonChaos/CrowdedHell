#-------------------------------------------------
#
# Project created by QtCreator 2018-12-02T01:09:40
#
#-------------------------------------------------

QT       += core gui opengl sql

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

win32:QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

CONFIG += c++17

INCLUDEPATH += \
        $$PWD/FMOD/inc

win32: LIBS += -L$$PWD/FMOD/lib/ -lfmod64_vc -lfmodL64_vc
else:unix: LIBS += -L$$PWD/FMOD/lib/ -lfmod -lfmodL

win32: LIBS += -lAdvapi32

unix: QT_CONFIG -= no-pkg-config
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

SOURCES += \
        main.cpp \
        crowdedhellgui.cpp \
    display/avoidancedisplaywidget.cpp \
    audio/audioplayerslider.cpp \
    audio/audioplayer.cpp \
    manager/resourcemanager.cpp \
    manager/projectmanager.cpp \
    manager/resourcetreeview.cpp \
    manager/createprojectwizard.cpp \
    barrage/normalbarrage.cpp \
    barrage/metagmobject.cpp \
    sqlite/sqlitedatabase.cpp \
    manager/addresourcewizard.cpp \
    resource/sprite.cpp

HEADERS += \
        crowdedhellgui.h \
    display/avoidancedisplaywidget.h \
    audio/audioplayerslider.h \
    audio/audioplayer.h \
    manager/projectmanager.h \
    manager/resourcemanager.h \
    manager/resourcetreeview.h \
    manager/createprojectwizard.h \
    barrage/normalbarrage.h \
    barrage/metagmobject.h \
    sqlite/sqlitedatabase.h \
    manager/addresourcewizard.h \
    resource/sprite.h \
    datastructrue/grid.hpp

FORMS += \
        crowdedhellgui.ui \
    manager/createprojectwizard.ui \
    manager/addresourcewizard.ui

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
    translation.qrc \
    DefalutTheme.qrc
