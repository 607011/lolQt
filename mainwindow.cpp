// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QMessageBox>
#include <QMovie>
#include <QMediaPlayer>
#include <QMediaContent>
#include <QMediaResource>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsform.h"
#include "consolewidget.h"

class MainWindowPrivate
{
public:
    MainWindowPrivate()
        : settingsForm(new SettingsForm)
        , imageWidget(new ImageWidget)
        , consoleWidget(new ConsoleWidget)
        , process(new QProcess)
        , movie(new QMovie)
        , audio(new QMediaPlayer)
        , originalFPS(0)
        , fps(0)
        , framesNeeded(0)
        , frameFilenamePattern("%1-%2.png")
    { /* ... */ }

    SettingsForm *settingsForm;
    ImageWidget *imageWidget;
    ConsoleWidget *consoleWidget;
    QProcess *process;
    QMovie *movie;
    QMediaPlayer *audio;
    QString audioFilename;
    QStringList tmpImageFiles;
    qreal originalFPS;
    qreal fps;
    int framesNeeded;
    // %1 original filename
    // %2 sequence number (4 digits)
    QString frameFilenamePattern;

    ~MainWindowPrivate()
    {
        delete movie;
        delete audio;
    }
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
    disableSave();

    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addWidget(d->imageWidget);
    ui->originalGroupBox->setLayout(hbox1);

    QObject::connect(ui->actionOpenImage, SIGNAL(triggered()), SLOT(openImage()));
    QObject::connect(ui->actionOpenAudio, SIGNAL(triggered()), SLOT(openAudio()));
    QObject::connect(ui->actionSaveFrames, SIGNAL(triggered()), SLOT(saveVideo()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered()), SLOT(close()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionSettings, SIGNAL(triggered()), SLOT(showSettings()));
    QObject::connect(ui->saveFramesButton, SIGNAL(clicked()), SLOT(saveVideo()));
    QObject::connect(d->imageWidget, SIGNAL(gifDropped(QString)), SLOT(analyzeMovie(QString)));
    QObject::connect(d->imageWidget, SIGNAL(musicDropped(QString)), SLOT(analyzeAudio(QString)));
    QObject::connect(d->audio, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    QObject::connect(ui->bpmSpinBox, SIGNAL(valueChanged(int)), SLOT(bpmChanged(int)));
    QObject::connect(d->process, SIGNAL(readyReadStandardOutput()), SLOT(processOutput()));
    QObject::connect(d->process, SIGNAL(readyReadStandardError()), SLOT(processErrorOutput()));
    QObject::connect(d->process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(processFinished(int,QProcess::ExitStatus)));

    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    for (int i = 100; i >= 0; i -= 10) {
        QAction *action = new QAction(QString("%1%").arg(i), this);
        action->setData(i);
        ui->menuVolume->addAction(action);
        QObject::connect(action, SIGNAL(triggered()), SLOT(setVolume()));
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    Q_D(MainWindow);
    if (d->process->state() == QProcess::Running) {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,
                                       tr("Really exit?"),
                                       tr("The encoder is running in the background."
                                          " If you quit, the process will be cancelled and the results be lost."
                                          " Do you really want to quit?"));
        if (button != QMessageBox::Yes)
            return e->ignore();
        d->process->close();
    }
    saveAppSettings();
    d->settingsForm->close();
    e->accept();
}


void MainWindow::openImage(void)
{
    Q_D(MainWindow);
    const QString &fileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Open animated GIF/PNG"),
                                         d->settingsForm->getOpenDirectory(),
                                         tr("Images (*.gif *.png)"));
    if (fileName.isEmpty())
        return;
    QFileInfo fi(fileName);
    d->settingsForm->setOpenDirectory(fi.absoluteFilePath());
    analyzeMovie(fileName);
}


void MainWindow::openAudio(void)
{
    Q_D(MainWindow);
    const QString &fileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Open audio file"),
                                         d->settingsForm->getOpenDirectory(),
                                         tr("Audio files (*.mp3)"));
    if (fileName.isEmpty())
        return;
    QFileInfo fi(fileName);
    d->settingsForm->setOpenDirectory(fi.absoluteFilePath());
    analyzeAudio(fileName);
}


void MainWindow::saveVideo(void)
{
    Q_D(MainWindow);
    QFileInfo fi(d->settingsForm->getOutputFile());
    if (fi.exists()) {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this,
                                       tr("Overwrite file?"),
                                       tr("The selected output file already exists."
                                          " Do you want to overwrite the file?"));
        if (button != QMessageBox::Yes)
            return;
    }
    QFile frameFileList(d->settingsForm->getTempDirectory() + "/" + AppName + "-list.txt");
    const int N = d->tmpImageFiles.count();
    Q_ASSERT(N > 0);
    if (frameFileList.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        for (int i = 0, j = 0; i < d->framesNeeded; ++i, ++j) {
            frameFileList.write(d->tmpImageFiles[j % N].toLocal8Bit());
            frameFileList.write("\n");
        }
    }
    frameFileList.close();
    const QImage &frame = d->movie->currentImage();
    QString cmdLine = d->settingsForm->getMencoderPath() +
            QString(" mf://@%1").arg(frameFileList.fileName()) +
            QString(" -mf w=%1:h=%2:fps=%3:type=png").arg(frame.width()).arg(frame.height()).arg(d->fps) +
            QString(" -ovc lavc") +
            QString(" -lavcopts vcodec=mpeg4:mbd=2:trell:aspect=%1/%2").arg(frame.width()).arg(frame.height()) +
            QString(" -oac mp3lame") +
            QString(" -lameopts cbr:br=%1").arg(d->settingsForm->getAudioBitrate()) +
            QString(" -audiofile \"%1\"").arg(d->audioFilename) +
            QString(" -o \"%1\"").arg(d->settingsForm->getOutputFile());
    disableSave();
    d->process->start(cmdLine);
    d->consoleWidget->clear();
    d->consoleWidget->show();
}


void MainWindow::processOutput(void)
{
    Q_D(MainWindow);
    const QByteArray &buf = d->process->readAllStandardOutput();
    ui->statusBar->showMessage(buf);
    d->consoleWidget->out(buf);
}


void MainWindow::processErrorOutput(void)
{
    Q_D(MainWindow);
    const QByteArray &buf = d->process->readAllStandardError();
    d->consoleWidget->out(buf);
}


void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_D(MainWindow);
    ui->statusBar->showMessage(tr("Written video to \"%1\".").arg(d->settingsForm->getOutputFile()));
    enableSave();
}


void MainWindow::setVolume(void)
{
    Q_D(MainWindow);
    QAction *action = reinterpret_cast<QAction*>(sender());
    if (action)
        d->audio->setVolume(action->data().toInt());
}


void MainWindow::calculateFPS(void)
{
    Q_D(MainWindow);
    if (d->movie->frameCount() > 0) {
        qreal bpm = ui->bpmSpinBox->value();
        d->fps = bpm / 60.0 * d->movie->frameCount();
        ui->fpsLineEdit->setText(QString("%1").arg(d->fps, 0, 'g', 4));
        d->movie->setSpeed(qRound(100 / d->originalFPS * d->fps));
        qreal duration = d->audio->duration() > 0 ? 1e-3 * d->audio->duration() : 1;
        d->framesNeeded = qRound(d->fps * duration);
    }
}


void MainWindow::durationChanged(qint64)
{
    calculateFPS();
}


void MainWindow::bpmChanged(int)
{
    calculateFPS();
}


void MainWindow::analyzeMovie(const QString &fileName)
{
    Q_D(MainWindow);
    d->movie->stop();
    d->movie->setFileName(fileName);
    QFileInfo fi(fileName);
    if (d->movie->isValid() && d->movie->frameCount() > 0) {
        d->tmpImageFiles.clear();
        qreal duration = 0;
        d->movie->jumpToFrame(0);
        for (int i = 0; i < d->movie->frameCount(); ++i) {
            const QString &fqSavePath = d->settingsForm->getTempDirectory() +
                    "/" +
                    d->frameFilenamePattern.arg(fi.baseName()).arg(i, 4, 10, QChar('0'));
            const QImage &frame = d->movie->currentImage();
            frame.save(fqSavePath);
            duration += d->movie->nextFrameDelay();
            d->tmpImageFiles.push_back(fqSavePath);
            d->movie->jumpToNextFrame();
        }
        d->originalFPS = 1000 * d->movie->frameCount() / duration;
        d->fps = d->originalFPS;
        ui->statusBar->showMessage(tr("%1 frames, %2 fps, %3 ms")
                                   .arg(d->movie->frameCount())
                                   .arg(qRound(d->originalFPS))
                                   .arg(int(duration)), 3000);
        enableSave();
        d->imageWidget->setMovie(d->movie);
        d->movie->start();
        calculateFPS();
    }
    else {
        disableSave();
    }
}


void MainWindow::analyzeAudio(const QString &fileName)
{
    Q_D(MainWindow);
    d->audioFilename = fileName;
    d->audio->setMedia(QUrl::fromLocalFile(fileName));
    d->audio->play();
    ui->statusBar->showMessage(tr("Audio loaded. Now set bpm accordingly!"), 10000);
}


void MainWindow::showSettings(void)
{
    Q_D(MainWindow);
    d->settingsForm->show();
}


void MainWindow::saveAppSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("Settings/outputFile", d->settingsForm->getOutputFile());
    settings.setValue("Settings/openDir", d->settingsForm->getOpenDirectory());
    settings.setValue("Settings/tmpDir", d->settingsForm->getTempDirectory());
    settings.setValue("Settings/mencoderPath", d->settingsForm->getMencoderPath());
    settings.setValue("Settings/audioBitrate", d->settingsForm->getAudioBitrate());
    settings.setValue("Settings/volume", d->audio->volume());
}


void MainWindow::restoreAppSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    d->settingsForm->setOutputFile(settings.value("Settings/outputFile", d->settingsForm->getOutputFile()).toString());
    d->settingsForm->setOpenDirectory(settings.value("Settings/openDir", d->settingsForm->getOpenDirectory()).toString());
    d->settingsForm->setTempDirectory(settings.value("Settings/tmpDir", d->settingsForm->getTempDirectory()).toString());
    d->settingsForm->setMencoderPath(settings.value("Settings/mencoderPath", d->settingsForm->getMencoderPath()).toString());
    d->settingsForm->setAudioBitrate(settings.value("Settings/audioBitrate", d->settingsForm->getAudioBitrate()).toInt());
    d->audio->setVolume(settings.value("Settings/volume", 50).toInt());
}


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About %1 %2%3").arg(AppName).arg(AppVersionNoDebug).arg(AppMinorVersion),
                       tr("<p><b>%1</b> merges animated GIFs and music into videos. "
                          "See <a href=\"%2\" title=\"%1 project homepage\">%2</a> for more info.</p>"
                          "<p>Copyright &copy; 2014 %3 &lt;%4&gt;, Heise Zeitschriften Verlag.</p>"
                          "<p>This program is free software: you can redistribute it and/or modify "
                          "it under the terms of the GNU General Public License as published by "
                          "the Free Software Foundation, either version 3 of the License, or "
                          "(at your option) any later version.</p>"
                          "<p>This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                          "GNU General Public License for more details.</p>"
                          "You should have received a copy of the GNU General Public License "
                          "along with this program. "
                          "If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0\">http://www.gnu.org/licenses</a>.</p>")
                       .arg(AppName).arg(AppUrl).arg(AppAuthor).arg(AppAuthorMail));
}


void MainWindow::enableSave(void)
{
    ui->saveFramesButton->setEnabled(true);
    ui->actionSaveFrames->setEnabled(true);
}


void MainWindow::disableSave(void)
{
    ui->saveFramesButton->setEnabled(false);
    ui->actionSaveFrames->setEnabled(false);
}
