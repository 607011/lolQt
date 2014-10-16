// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QtCore/QDebug>

const QString Company = "c't";
const QString AppName = "lolQt";
const QString AppUrl = "https://github.com/ola-ct/lolqt";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
const QString AppVersionNoDebug = "1.0.4";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug;
#else
const QString AppVersion = AppVersionNoDebug + " [DEBUG]";
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersionNoDebug);

    qApp->addLibraryPath("plugins");
    qApp->addLibraryPath("./plugins");


#ifdef Q_OS_MAC
    qApp->addLibraryPath("../plugins");
#endif

#ifndef QT_NO_DEBUG
    qDebug() << qApp->libraryPaths();
#endif

    QTranslator translator;
    QString translationFile = QString(":/translations/lolqt-%1").arg(QLocale::system().name());
    bool ok = translator.load(translationFile);
    if (ok) {
        ok = a.installTranslator(&translator);
        if (!ok)
            qWarning() << "Could not install translator for" << QLocale::system().name() << "locale";
    }
    else {
        qWarning() << "Could not load translations for" << QLocale::system().name() << "locale";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
