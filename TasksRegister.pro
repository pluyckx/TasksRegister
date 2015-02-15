#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T07:37:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TasksRegister
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timecontainer.cpp \
    task.cpp \
    tasksmodel.cpp \
    hotkeyeventfilter.cpp \
    deletekeyeventfilter.cpp

linux-g++ {
    SOURCES +=
}
win32 {
    SOURCES +=
    CONFIG += console
}

HEADERS  += mainwindow.h \
    timecontainer.h \
    task.h \
    tasksmodel.h \
    hotkeyeventfilter.h \
    deletekeyeventfilter.h

RESOURCES +=
