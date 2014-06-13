# Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
# All rights reserved.

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GifExploder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp \
    settingsform.cpp \
    consolewidget.cpp \
    wavewidget.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    main.h \
    settingsform.h \
    consolewidget.h \
    wavewidget.h

FORMS    += mainwindow.ui \
    settingsform.ui \
    consolewidget.ui \
    wavewidget.ui

OTHER_FILES += \
    .gitignore \
    README.md
