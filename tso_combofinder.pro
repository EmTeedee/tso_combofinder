#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T13:37:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tso_combofinder
TEMPLATE = app


SOURCES += main.cpp\
        tsofinder.cpp\
        items.cpp \
        ui_options.cpp \
        ui_functions.cpp \
        settings.cpp \
        events.cpp

HEADERS  += tsofinder.h

FORMS    += tsofinder.ui

DISTFILES +=

RESOURCES += \
    images.qrc\
    languages.qrc

TRANSLATIONS = \
    languages/tsofinder_en.ts\
    languages/tsofinder_de.ts

QMAKE_TARGET_DESCRIPTION = "The Settlers Online - Collectible Finder"
win32:RC_ICONS += settler.ico
