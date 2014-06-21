#-------------------------------------------------
#
# Project created by QtCreator 2014-05-23T19:00:00
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = lib

SOURCES += myplugin.cpp

HEADERS += myplugin.h

CONFIG         += plugin

INCLUDEPATH    += ../Core/headers
TARGET          = $$qtLibraryTarget(myplugin)
DESTDIR         = ../plugins

EXAMPLE_FILES = myplugin.json
