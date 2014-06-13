// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __SETTINGSFORM_H_
#define __SETTINGSFORM_H_

#include <QDialog>

namespace Ui {
class SettingsForm;
}

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
    int getAudioBitrate(void) const;
    void setAudioBitrate(int);

public slots:
    bool chooseOutputFile(void);
    void chooseOpenDirectory(void);
    void chooseTempDirectory(void);
    void chooseMencoder(void);

private: // methods

private: // variables
    Ui::SettingsForm *ui;

};

#endif // __SETTINGSFORM_H_
