// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "settingsform.h"
#include "ui_settingsform.h"
#include "main.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QDebug>

class SettingsFormPrivate {
public:
    SettingsFormPrivate(void)
        : dirty(false)
    { /* ... */ }
    bool dirty;
};

SettingsForm::SettingsForm(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsForm)
    , d_ptr(new SettingsFormPrivate)
{
    ui->setupUi(this);

    setModal(true);
    setWindowTitle(QString("%1 - Settings").arg(AppName));

    ui->mencoderPathLineEdit->setText(QDir::currentPath() + "/mencoder.exe");
    ui->outputFileLineEdit->setText(QDir::homePath() + "/output.avi");
    ui->openDirectoryLineEdit->setText(QDir::homePath());
    ui->tmpDirectoryLineEdit->setText(QDir::tempPath());
    ui->mencoderOptionsPlainTextEdit->setPlainText("-mf w=%1:h=%2:fps=%3:type=png"
                                         " -of avi"
                                         " -ovc lavc"
                                         " -lavcopts vcodec=mpeg4:mbd=2:trell:v4mv:mv0:dia=2:aspect=%4/%5"
                                         " -oac mp3lame"
                                         " -lameopts cbr:br=%6");
    ui->subtitleFontLineEdit->setText(QDir::currentPath() + "/big_noodle_titling.ttf");

    QObject::connect(ui->chooseOpenDirectoryPushButton, SIGNAL(clicked()), SLOT(chooseOpenDirectory()));
    QObject::connect(ui->chooseOutputFilePushButton, SIGNAL(clicked()), SLOT(chooseOutputFile()));
    QObject::connect(ui->chooseTmpDirectoryPushButton, SIGNAL(clicked()), SLOT(chooseTempDirectory()));
    QObject::connect(ui->chooseMencoderPushButton, SIGNAL(clicked()), SLOT(chooseMencoder()));
    QObject::connect(ui->closePushButton, SIGNAL(clicked()), SLOT(close()));
    QObject::connect(ui->mencoderPresetsComboBox, SIGNAL(currentIndexChanged(int)), SLOT(mencoderPresetChanged(int)));
    QObject::connect(ui->mencoderOptionsPlainTextEdit, SIGNAL(textChanged()), SLOT(mencoderOptionsChanged()));

    // see https://wiki.archlinux.org/index.php/MEncoder
    ui->mencoderPresetsComboBox->addItem(QString(), QVariant());
    ui->mencoderPresetsComboBox->addItem(
                tr("high quality MPEG4 (lavc) encoding"),
                "-mf w=%1:h=%2:fps=%3:type=png "
                "-of avi -ovc lavc "
                "-lavcopts vcodec=mpeg4:mbd=2:trell:v4mv:mv0:dia=2:aspect=%4/%5 "
                "-oac copy");
    ui->mencoderPresetsComboBox->addItem(
                tr("high quality H.264 (x264) encoding"),
                "-mf w=%1:h=%2:fps=%3:type=png "
                "-oac copy -ovc x264 "
                "-x264encopts preset=veryslow:tune=film:crf=15:frameref=15:fast_pskip=0:threads=%7 "
                "-ofps %3");
    qDebug() << "EOF SettingsForm::SettingsForm()";
    d_ptr->dirty = false;
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


QString SettingsForm::getMEncoderOptions(void) const
{
    return ui->mencoderOptionsPlainTextEdit->toPlainText().replace("\n", " ");
}


void SettingsForm::setMEncoderOptions(const QString &mencoderOptions)
{
    ui->mencoderOptionsPlainTextEdit->setPlainText(mencoderOptions);
}


QString SettingsForm::getSubtitleFont(void) const
{
    return ui->subtitleFontLineEdit->text();
}

void SettingsForm::setSubtitleFont(const QString &subtitleFont)
{
    ui->subtitleFontLineEdit->setText(subtitleFont);
}


int SettingsForm::getAudioBitrate() const
{
    return ui->audioBitrateSpinBox->value();
}


void SettingsForm::setAudioBitrate(int kbps)
{
    ui->audioBitrateSpinBox->setValue(kbps);
}


bool SettingsForm::getSubtitlesEnabled() const
{
    return ui->showArtistTitleCheckBox->isChecked();
}


void SettingsForm::setSubtitlesEnabled(bool enabled)
{
    ui->showArtistTitleCheckBox->setChecked(enabled);
}


bool SettingsForm::chooseOutputFile(void)
{
    const QString &outDir =
            QFileDialog::getSaveFileName(this,
                                         tr("Select output file"),
                                         ui->outputFileLineEdit->text());
    if (outDir.isEmpty())
        return false;
    ui->outputFileLineEdit->setText(outDir);
    return true;
}


void SettingsForm::chooseOpenDirectory(void)
{
    const QString &outDir =
            QFileDialog::getExistingDirectory(
                this,
                tr("Select open directory"),
                ui->openDirectoryLineEdit->text());
    if (outDir.isEmpty())
        return;
    ui->openDirectoryLineEdit->setText(outDir);
}


void SettingsForm::chooseTempDirectory(void)
{
    const QString &outDir =
            QFileDialog::getExistingDirectory(
                this,
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


void SettingsForm::mencoderPresetChanged(int idx)
{
    Q_D(SettingsForm);
    bool doOverwrite = true;
    if (d->dirty) {
        QMessageBox::StandardButton choice = QMessageBox::question(
                    this,
                    tr("Really overwrite changes?"),
                    tr("You've made changes to the MEncoder options. "
                       "Do you really want to overwrite them by choosing a preset?"));
        if (choice != QMessageBox::Yes)
            doOverwrite = false;
    }
    if (doOverwrite) {
        const QVariant &data = ui->mencoderPresetsComboBox->itemData(idx);
        if (!data.isNull()) {
            ui->mencoderOptionsPlainTextEdit->setPlainText(data.toString());
            d->dirty = false;
        }
    }
}


void SettingsForm::mencoderOptionsChanged(void)
{
    Q_D(SettingsForm);
    qDebug() << "SettingsForm::mencoderOptionsChanged()";
    d->dirty = true;
}
