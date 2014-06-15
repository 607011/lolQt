// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "fft.h"
#include "energywidget.h"
#include <QPainter>
#include <QVector>
#include <QtConcurrent>
#include <QFuture>
#include <QElapsedTimer>
#include <QPainterPath>
#include <QtCore/QDebug>
#include "types.h"

class EnergyWidgetPrivate {
public:
    EnergyWidgetPrivate(void)
        : maxEnergy(0)
        , spectrum(EnergyWidget::BinSize, 0)
    { /* ... */ }

    qreal maxEnergy;
    SampleBuffer samples;
    QVector<qreal> spectrum;
    QFuture<void> analyzeFuture;
};


EnergyWidget::EnergyWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new EnergyWidgetPrivate)
{
    // ...
}


EnergyWidget::~EnergyWidget()
{
    // ...
}


void EnergyWidget::analyzeSamples(void)
{
    Q_D(EnergyWidget);
    QElapsedTimer timer;
    timer.start();

    qreal *xsamples = new qreal[BinSize];
    qreal *ysamples = new qreal[BinSize];

    SampleBuffer::const_iterator src = d->samples.constBegin();
    int j = d->samples.count();
    while (true) {
        j -= BinSize;
        if (j <= 0)
            break;
        qreal *dst = xsamples;
        for (int i = 0; i < BinSize; ++i)
            *dst++ = *src++;
        memset(ysamples, 0, sizeof(qreal) * BinSize);
        FFT::DFT(FFT::Forward, BinSize, xsamples, ysamples);
        qreal maxEnergy = 0;
        for (int i = 0; i < BinSize; ++i) {
            if (xsamples[i] > maxEnergy)
                maxEnergy = xsamples[i];
        }
        qDebug() << j << maxEnergy;
        for (int i = 0; i < BinSize; ++i)
            d->spectrum[i] = ysamples[i];
        update();
    }

    delete [] xsamples;
    delete [] ysamples;
    qDebug() << "EnergyWidget::setSamples() took" << timer.elapsed() << "ms.";

}


void EnergyWidget::setSamples(SampleBuffer samples)
{
    Q_D(EnergyWidget);
    d->samples = samples;
    d->analyzeFuture = QtConcurrent::run(this, &EnergyWidget::analyzeSamples);
}


void EnergyWidget::paintEvent(QPaintEvent*)
{
    Q_D(EnergyWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::darkGray);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::green);
    qreal xs = qreal(width()) / BinSize;
    qreal ys = 1e-3 * height();
    QPainterPath path;
    static const int xd = 4;
    for (int i = 0; i < BinSize / xd; ++i)
        path.addRect(xd * i * xs, height(), xd * xs, -qAbs(d->spectrum[i] * ys));
    p.drawPath(path);
}

