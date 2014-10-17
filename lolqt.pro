# Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
# All rights reserved.

QT       += core gui multimedia concurrent

VERSION = 1.0.5

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lolQt
TEMPLATE = app

TRANSLATIONS = lolqt-de_DE.ts

CODECFORTR = UTF-8

win32 {
RC_FILE = lolqt.rc
}

SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp \
    settingsform.cpp \
    consolewidget.cpp \
    wavewidget.cpp \
    energywidget.cpp \
    kiss_fft.c

HEADERS  += mainwindow.h \
    imagewidget.h \
    main.h \
    settingsform.h \
    consolewidget.h \
    wavewidget.h \
    energywidget.h \
    types.h \
    kiss_fft.h \
    _kiss_fft_guts.h \
    fft.h

FORMS    += mainwindow.ui \
    settingsform.ui \
    consolewidget.ui

OTHER_FILES += \
    .gitignore \
    README.md \
    LICENSE \
    deploy/lolqt.nsi \
    lolqt.rc \
    LIESMICH.md \
    lolqt-de_DE.ts

RESOURCES += \
    lolqt.qrc
