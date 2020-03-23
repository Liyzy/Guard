#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T14:37:08
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = GUARD
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    TowerPosition.cpp \
    Tower.cpp \
    waypoint.cpp \
    enemy.cpp \
    bullet.cpp \
    music.cpp \
    clickposition.cpp

HEADERS  += mainwindow.h \
    TowerPosition.h \
    Tower.h \
    WayPoint.h \
    enemy.h \
    collide.h \
    bullet.h \
    music.h \
    clickposition.h

FORMS    += mainwindow.ui

RESOURCES +=
