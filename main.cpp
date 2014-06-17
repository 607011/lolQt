// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "mainwindow.h"
#include <QApplication>

const QString Company = "c't";
const QString AppName = "lolct";
const QString AppUrl = "https://github.com/ola-ct/lolct";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
const QString AppVersionNoDebug = "1.0";
const QString AppMinorVersion = ".0";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
