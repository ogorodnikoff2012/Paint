#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T19:18:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paint
TEMPLATE = app


SOURCES += main.cpp \
	widgets/paintworkspace.cpp \
	widgets/mainwindow.cpp \
	instruments/pencil.cpp \
#    tests/testwidget.cpp
	widgets/workspaceshell.cpp \
	widgets/verticalruler.cpp \
	widgets/horizontalruler.cpp \
    widgets/palette.cpp

HEADERS  += \
	headers/paintworkspace.h \
	headers/mainwindow.h \
	headers/plugin.h \
	headers/instrument.h \
	headers/pencil.h \
#    headers/testwidget.h
	headers/workspaceshell.h \
	headers/verticalruler.h \
	headers/ruler.h \
	headers/horizontalruler.h \
    headers/palette.h

win32 {
	CONFIG(debug, release|debug):DESTDIR = ../debug/
	CONFIG(release, release|debug):DESTDIR = ../release/
} else {
	DESTDIR    = ../
}

FORMS += \
	widgets/mainwindow.ui

RESOURCES += \
	icons.qrc

TRANSLATIONS = translations/ru.ts
