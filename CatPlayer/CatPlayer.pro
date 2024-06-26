#-------------------------------------------------
#
# Project created by QtCreator 2024-05-10T10:58:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CatPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS #__STDC_CONSTANT_MACROS
DEFINES += __STDC_CONSTANT_MACROS
# __STDC_CONSTANT_MACROS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    TitleBar.cpp \
    CtrlBar.cpp \
    MediaCentre.cpp \
    MainWindow.cpp \
    AvMsgQueue.cpp \
    ../player/MyPlayer.cpp \
    ../player/AvPlayDef.cpp

HEADERS += \
    TitleBar.h \
    CtrlBar.h \
    MediaCentre.h \
    AvMsgQueue.h \
    MainWindow.h \
    ../player/MyPlayer.h \
    ../player/AudioVideoState.h \
    ../player/AvPlayDef.h

FORMS += \
        Mainwindow.ui \
    TitleBar.ui \
    CtrlBar.ui

win32 {

INCLUDEPATH += ../include/ffmpeg4.2.1 \
               ../include/sdl2.0.10  \
               ../player

LIBS += -L$$PWD/../third_lib/ffmpeg4.2.1   \
        -L$$PWD/../third_lib/sdl2.0.10/x86  \
        -lavutil  \
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lpostproc  \
        -lswresample \
        -lswscale   \
        -lSDL2
}
