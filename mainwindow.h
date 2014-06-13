// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QProcess>
#include <QScopedPointer>
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

private: // methods
    void saveAppSettings(void);
    void restoreAppSettings(void);
    void enableSave(void);
    void disableSave(void);
    void calculateFPS(void);

private:
    Ui::MainWindow *ui;

    QScopedPointer<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
    Q_DISABLE_COPY(MainWindow)

private slots:
    void openImage(void);
    void saveVideo(void);
    void about(void);
    void analyzeMovie(const QString& fileName);
    void analyzeAudio(const QString& fileName);
    void durationChanged(qint64 ms = -1);
    void bpmChanged(int);
    void processOutput(void);
    void processErrorOutput(void);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void setVolume(void);
};

#endif // __MAINWINDOW_H_
