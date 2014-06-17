// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __ENERGYWIDGET_H_
#define __ENERGYWIDGET_H_

#include <QWidget>
#include <QPaintEvent>
#include <QAudioBuffer>
#include <QScopedPointer>
#include "types.h"

class EnergyWidgetPrivate;

class EnergyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EnergyWidget(QWidget *parent = 0);
    ~EnergyWidget();
    QSize sizeHint(void) const { return QSize(128, 128); }
    QSize minimumSizeHint(void) const { return QSize(128, 64); }
    void setSamples(const SampleBuffer&);

    static const int BinSize = 256;
    static const int NBins = BinSize / 2 + 1;

    bool isActive(void) const;
    void cancel(void);


public slots:
    void setPosition(qint64);
    void setDuration(qint64);

signals:

protected:
    void paintEvent(QPaintEvent*);

private: // methods
    void analyzeSamples(void);

private:

    QScopedPointer<EnergyWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(EnergyWidget)
    Q_DISABLE_COPY(EnergyWidget)

};

#endif // __ENERGYWIDGET_H_
