// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QMovie>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"


class MainWindowPrivate
{
public:
    MainWindowPrivate()
        : imageWidget(0)
        , frameFilenamePattern("%1-%2.png")
    { /* ... */ }

    ImageWidget *imageWidget;
    QString saveDir;
    // %1 original filename
    // %2 sequence number (4 digits)
    QString frameFilenamePattern;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , d_ptr(new MainWindowPrivate())
{
    Q_D(MainWindow);
    ui->setupUi(this);

    QSettings::setDefaultFormat(QSettings::NativeFormat);
    restoreAppSettings();

    d->imageWidget = new ImageWidget;
    QHBoxLayout* hbox1 = new QHBoxLayout;
    hbox1->addWidget(d->imageWidget);
    ui->originalGroupBox->setLayout(hbox1);

    QObject::connect(ui->actionSaveFrames, SIGNAL(triggered()), this, SLOT(saveFrames()));
    QObject::connect(ui->saveFramesButton, SIGNAL(clicked()), this, SLOT(saveFrames()));
    QObject::connect(d->imageWidget, SIGNAL(gifDropped(QString)), this, SLOT(saveFrames()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    saveAppSettings();
    e->accept();
}


void MainWindow::saveFrames(void)
{
    Q_D(MainWindow);
    if (d->saveDir.isEmpty()) {
        const QString& outDir =
                QFileDialog::getExistingDirectory(this,
                                                  tr("Select save directory"),
                                                  d->saveDir);
        if (outDir.isEmpty())
            return;
        d->saveDir = outDir;
    }
    const QString &originalFilename = d->imageWidget->imageFileName();
    QFileInfo fi(originalFilename);
    QMovie gif(originalFilename);
    if (gif.isValid() && gif.frameCount() > 0) {
        gif.jumpToFrame(0);
        for (int i = 0; i < gif.frameCount(); ++i) {
            QImage frame = gif.currentImage();
            QString fqSavePath = d->saveDir + "/" + d->frameFilenamePattern.arg(fi.baseName()).arg(i, 4, 10, QChar('0'));
            frame.save(fqSavePath);
            gif.jumpToNextFrame();
        }
    }
}


void MainWindow::saveAppSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("MainWindow/saveDir", d->saveDir);
}


void MainWindow::restoreAppSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    d->saveDir = settings.value("MainWindow/saveDir").toString();
}
