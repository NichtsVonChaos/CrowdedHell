#-------------------------------------------------
#
# Project created by QtCreator 2019-06-03T13:21:12
#
#-------------------------------------------------

QT       += core gui sql opengl

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$PWD/Reference/QtShell/qtshell.pri)
include($$PWD/Reference/SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

win32: LIBS += \
    -L$$quote($$PWD/Reference/FMOD/lib/Windows x64) -lfmod_vc -lfmodL_vc
else:unix: LIBS += \
    -L$$quote($$PWD/Reference/FMOD/lib/Linux x64) -lfmod -lfmodL

TRANSLATIONS = \
    Resources/Translations/en.ts \
    Resources/Translations/zh_cn.ts \
    Resources/Translations/zh_tw.ts \
    Resources/Translations/jp.ts

INCLUDEPATH += \
    Reference

FORMS += \
    Codes/MainWindow/mainwindow.ui \
    Codes/MainWindow/terminal.ui \
    Codes/Wizards/createprojectwizard.ui

HEADERS += \
    Codes/DataStructure/grid.hpp \
    Codes/GlobalComponent/commandlinesystem.h \
    Codes/GlobalComponent/customfunctions.h \
    Codes/GlobalComponent/globalComponent.h \
    Codes/GlobalComponent/logger.h \
    Codes/GlobalComponent/options.h \
    Codes/GlobalComponent/project.h \
    Codes/MainWindow/commandlineedit.h \
    Codes/MainWindow/mainwindow.h \
    Codes/MainWindow/musicslider.h \
    Codes/MainWindow/terminal.h \
    Codes/MainWindow/volumeslider.h \
    Codes/ResourceManager/Image/image.h \
    Codes/ResourceManager/Image/mask.h \
    Codes/ResourceManager/Image/sprite.h \
    Codes/ResourceManager/Music/musicplayer.h \
    Codes/ResourceManager/Instance/metagminstance.h \
    Codes/ResourceManager/Path/path.h \
    Codes/ResourceManager/gmglobal.h \
    Codes/SQLite/sqlitedatabase.h \
    Codes/Wizards/createprojectwizard.h

SOURCES += \
    Codes/GlobalComponent/commandlinesystem.cpp \
    Codes/GlobalComponent/logger.cpp \
    Codes/GlobalComponent/options.cpp \
    Codes/GlobalComponent/project.cpp \
    Codes/MainWindow/commandlineedit.cpp \
    Codes/MainWindow/mainwindow.cpp \
    Codes/MainWindow/musicslider.cpp \
    Codes/MainWindow/terminal.cpp \
    Codes/MainWindow/volumeslider.cpp \
    Codes/ResourceManager/Image/image.cpp \
    Codes/ResourceManager/Image/mask.cpp \
    Codes/ResourceManager/Image/sprite.cpp \
    Codes/ResourceManager/Music/musicplayer.cpp \
    Codes/ResourceManager/Instance/metaobject.cpp \
    Codes/ResourceManager/Path/path.cpp \
    Codes/ResourceManager/gmglobal.cpp \
    Codes/SQLite/sqlitedatabase.cpp \
    Codes/Wizards/createprojectwizard.cpp \
    Codes/main.cpp

RESOURCES += \
    Reference/FMOD/FMOD_LOGOS/FMOD_LOGO.qrc \
    Resources/Translations/translations.qrc
