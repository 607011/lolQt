// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __WAVEWIDGET_H_
#define __WAVEWIDGET_H_

#include <QWidget>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QAudioBuffer>
#include <QScopedPointer>
#include <QVector>
#include "types.h"


class WaveWidgetPrivate;

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveWidget(QWidget *parent = 0);
    ~WaveWidget();
    QSize minimumSizeHint(void) const { return QSize(128, 64); }
    QSize sizeHint(void) const { return QSize(256, 128); }
    bool isActive(void) const;
    void setSamples(const SampleBuffer &samples, qint64 duration);
    void cancel(void);

public slots:
    void setPosition(qint64);
    void setDuration(qint64);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);

private: // methods
    void drawWaveForm(void);

private:
    QScopedPointer<WaveWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(WaveWidget)
    Q_DISABLE_COPY(WaveWidget)

};

#endif // __WAVEWIDGET_H_
