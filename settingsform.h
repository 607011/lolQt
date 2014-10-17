// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __SETTINGSFORM_H_
#define __SETTINGSFORM_H_

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class SettingsForm;
}

class SettingsFormPrivate;

class SettingsForm : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsForm(QDialog *parent = 0);
    ~SettingsForm();

    QString getOpenDirectory(void) const;
    void setOpenDirectory(const QString&);
    QString getOutputFile(void) const;
    void setOutputFile(const QString&);
    QString getTempDirectory(void) const;
    void setTempDirectory(const QString&);
    QString getMencoderPath(void) const;
    void setMencoderPath(const QString&);
    QString getMEncoderOptions(void) const;
    void setMEncoderOptions(const QString&);
    QString getSubtitleFont(void) const;
    void setSubtitleFont(const QString&);
    int getAudioBitrate(void) const;
    void setAudioBitrate(int);
    bool getSubtitlesEnabled(void) const;
    void setSubtitlesEnabled(bool);

public slots:
    bool chooseOutputFile(void);
    void chooseOpenDirectory(void);
    void chooseTempDirectory(void);
    void chooseMencoder(void);

private slots:
    void mencoderPresetChanged(int);
    void mencoderOptionsChanged(void);

private: // methods

private: // variables
    Ui::SettingsForm *ui;

    QScopedPointer<SettingsFormPrivate> d_ptr;
    Q_DECLARE_PRIVATE(SettingsForm)
    Q_DISABLE_COPY(SettingsForm)
};

#endif // __SETTINGSFORM_H_
