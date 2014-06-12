# Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
# All rights reserved.

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GifExploder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp

HEADERS  += mainwindow.h \
    imagewidget.h \
    main.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    .gitignore
