// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "settingsform.h"
#include "ui_settingsform.h"
#include "main.h"

#include <QDir>
#include <QFileDialog>

SettingsForm::SettingsForm(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsForm)
{
    ui->setupUi(this);

    setModal(true);
    setWindowTitle(QString("%1 - Settings").arg(AppName));

    ui->outputFileLineEdit->setText(QDir::currentPath() + "/output.avi");
    ui->openDirectoryLineEdit->setText(QDir::currentPath());
    ui->tmpDirectoryLineEdit->setText(QDir::tempPath());

    QObject::connect(ui->chooseOpenDirectoryPushButton, SIGNAL(clicked()), SLOT(chooseOpenDirectory()));
    QObject::connect(ui->chooseOutputFilePushButton, SIGNAL(clicked()), SLOT(chooseOutputFile()));
    QObject::connect(ui->chooseTmpDirectoryPushButton, SIGNAL(clicked()), SLOT(chooseTempDirectory()));
    QObject::connect(ui->chooseMencoderPushButton, SIGNAL(clicked()), SLOT(chooseMencoder()));
    QObject::connect(ui->closePushButton, SIGNAL(clicked()), SLOT(close()));
}


SettingsForm::~SettingsForm()
{
    delete ui;
}


QString SettingsForm::getOpenDirectory(void) const
{
    return ui->openDirectoryLineEdit->text();
}


void SettingsForm::setOpenDirectory(const QString &path)
{
    ui->openDirectoryLineEdit->setText(path);
}


QString SettingsForm::getOutputFile(void) const
{
    return ui->outputFileLineEdit->text();
}


void SettingsForm::setOutputFile(const QString &path)
{
    ui->outputFileLineEdit->setText(path);
}


QString SettingsForm::getTempDirectory(void) const
{
    return ui->tmpDirectoryLineEdit->text();
}


void SettingsForm::setTempDirectory(const QString &path)
{
    ui->tmpDirectoryLineEdit->setText(path);
}


QString SettingsForm::getMencoderPath(void) const
{
    return ui->mencoderPathLineEdit->text();
}


void SettingsForm::setMencoderPath(const QString &path)
{
    ui->mencoderPathLineEdit->setText(path);
}


int SettingsForm::getAudioBitrate() const
{
    return ui->audioBitrateSpinBox->value();
}


void SettingsForm::setAudioBitrate(int kbps)
{
    ui->audioBitrateSpinBox->setValue(kbps);
}


void SettingsForm::chooseOutputFile(void)
{
    const QString &outDir =
            QFileDialog::getSaveFileName(this,
                                         tr("Select output file"),
                                         ui->outputFileLineEdit->text());
    if (outDir.isEmpty())
        return;
    ui->outputFileLineEdit->setText(outDir);
}


void SettingsForm::chooseOpenDirectory(void)
{
    const QString &outDir =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select open directory"),
                                              ui->openDirectoryLineEdit->text());
    if (outDir.isEmpty())
        return;
    ui->openDirectoryLineEdit->setText(outDir);
}


void SettingsForm::chooseTempDirectory(void)
{
    const QString &outDir =
            QFileDialog::getExistingDirectory(this,
                                              tr("Select directory for temporary files"),
                                              ui->tmpDirectoryLineEdit->text());
    if (outDir.isEmpty())
        return;
    ui->tmpDirectoryLineEdit->setText(outDir);
}


void SettingsForm::chooseMencoder(void)
{
    const QString &path =
            QFileDialog::getOpenFileName(this, tr("Select mencoder binary"));
    if (path.isEmpty())
        return;
    ui->mencoderPathLineEdit->setText(path);
}
