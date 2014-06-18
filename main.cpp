// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QtCore/QDebug>

const QString Company = "c't";
const QString AppName = "lolQt";
const QString AppUrl = "https://github.com/ola-ct/lolqt";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
const QString AppVersionNoDebug = "1.0";
const QString AppMinorVersion = ".1";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qApp->addLibraryPath("plugins");
    qApp->addLibraryPath("./plugins");

#ifdef Q_OS_MAC
    qApp->addLibraryPath("../plugins");
#endif

#ifndef QT_NO_DEBUG
    qDebug() << qApp->libraryPaths();
#endif

    QTranslator translator;
    bool ok = translator.load(":/translations/lolqt-" + QLocale::system().name());
#ifndef QT_NO_DEBUG
    if (!ok)
        qWarning() << "Could not load translations for" << QLocale::system().name() << "locale";
#endif
    if (ok)
        a.installTranslator(&translator);

    w.show();
    return a.exec();
}
