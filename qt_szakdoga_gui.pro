#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T20:20:15
#
#-------------------------------------------------

QT       += core gui

#CONFIG += c++11 console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_szakdoga_gui
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

INCLUDEPATH += ./headers
INCLUDEPATH += ./sources

SOURCES += \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/camera.cpp \
    sources/glwidget.cpp \
    sources/frame.cpp \
    sources/framereader.cpp \
    sources/framesystem.cpp \
    sources/colorbutton.cpp \
    sources/functions.cpp \
    sources/materialcoloringcombobox.cpp \
    sources/openfiledialog.cpp \
    sources/customfilesystemmodel.cpp \
    sources/treeitem.cpp \
    sources/fileentry.cpp \
    sources/shaderloader.cpp \
    sources/openglinterface.cpp

HEADERS += \
    headers/mainwindow.h \
    headers/camera.h \
    headers/glwidget.h \
    headers/frame.h \
    headers/framereader.h \
    headers/framesystem.h \
    headers/colorbutton.h \
    headers/functions.h \
    headers/materialcoloringcombobox.h \
    headers/openfiledialog.h \
    headers/customfilesystemmodel.h \
    headers/treeitem.h \
    headers/fileentry.h \
    headers/shaderloader.h \
    headers/openglinterface.h


FORMS += \
        mainwindow.ui \
    openfiledialog.ui

RESOURCES += \
    resource.qrc
