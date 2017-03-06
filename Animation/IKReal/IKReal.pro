#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T14:26:34
#
#-------------------------------------------------

QT       += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IKReal
TEMPLATE = app
LIBS = -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    body.cpp \
    bvh.cpp \
    camera.cpp \
    light.cpp \
    movable.cpp \
    operateur.cpp \
    myglwidget.cpp \
    mywindow.cpp

HEADERS  += mainwindow.h \
    body.hpp \
    bvh.hpp \
    camera.hpp \
    light.hpp \
    main.hpp \
    movable.hpp \
    operateur.hpp \
    myglwidget.h \
    mywindow.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    IKReal.pro.user \
    Take 006.bvh \
    Makefile
