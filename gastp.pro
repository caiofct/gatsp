# -------------------------------------------------
# Project created by QtCreator 2009-09-22T21:11:31
# -------------------------------------------------
QT       += core gui uitools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gastp
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    individuo.cpp \
    cidade.cpp \
    genetico.cpp \
    graphicsScene.cpp \
    changePreferences.cpp
HEADERS += mainwindow.h \
    individuo.h \
    cidade.h \
    genetico.h \
    graphicsScene.h \
    changePreferences.h
FORMS += mainwindow.ui \
    configDialog.ui
