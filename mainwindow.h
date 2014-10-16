// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QProcess>
#include <QScopedPointer>
#include <QAudioBuffer>
#include <QAudioDecoder>
#include "imagewidget.h"
#include "main.h"

namespace Ui {
class MainWindow;
}

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void showSettings(void);
    void openImage(void);
    void openAudio(void);
    void saveVideoAs(void);
    void onSaveCancelClicked(void);
    void about(void);
    void analyzeMovie(const QString &fileName);
    void analyzeAudio(const QString &fileName);
    void durationChanged(qint64);
    void bpmChanged(double);
    void processOutput(void);
    void processErrorOutput(void);
    void processFinished(int, QProcess::ExitStatus);
    void audioBufferReady(const QAudioBuffer&);
    void metaDataAvailableChanged(bool);
    void setVolume(void);
    void showConsole(bool);
    void consoleClosed(void);
    void readAudioBuffer(void);
    void finishedAudioBuffer(void);
    void countBeat(void);
    void analysisCompleted(void);

private: // methods
    void cancelEncoding(void);
    void saveVideo(void);
    void saveAppSettings(void);
    void restoreAppSettings(void);
    void createProcess(void);
    void deleteProcess(void);
    bool processAllowedToBeCanceled(void);
    void enableSave(void);
    void disableSave(void);
    void calculateFPS(void);
    void cancelAudioAnalysis(void);
    QString getSubtitleFilename(void) const;
    QString getFrameFileListFilename(void) const;
    void removeTemporaryFiles(void);

private: // variables
    Ui::MainWindow *ui;

    QScopedPointer<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
    Q_DISABLE_COPY(MainWindow)

};

#endif // __MAINWINDOW_H_
