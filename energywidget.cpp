// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include "energywidget.h"
#include <QPainter>
#include <QVector>
#include <QtConcurrent>
#include <QFuture>
#include <QElapsedTimer>
#include <QPainterPath>
#include <QtCore/QDebug>
#include "types.h"
#include "fft.h"

class EnergyWidgetPrivate {
public:
    EnergyWidgetPrivate(void)
        : maxEnergy(0)
        , spectrum(EnergyWidget::NBins, 0)
        , spectrum2(EnergyWidget::NBins, 0)
        , doCancel(false)
        , percentReady(0)
    { /* ... */ }

    qreal maxEnergy;
    SampleBuffer samples;
    QVector<qreal> spectrum;
    QVector<qreal> spectrum2;
    QFuture<void> analyzeFuture;
    bool doCancel;
    int percentReady;
};


EnergyWidget::EnergyWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new EnergyWidgetPrivate)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
}


EnergyWidget::~EnergyWidget()
{
    // ...
}


void EnergyWidget::analyzeSamples(void)
{
    Q_D(EnergyWidget);
    QTime t0;
    t0.start();
    FFT<qint16> fft(BinSize);
    kiss_fft_cpx spectrum[BinSize];
    SampleBuffer::const_iterator src = d->samples.constBegin();
    d->percentReady = 0;
    int j = 0;
    while (!d->doCancel) {
        j += BinSize;
        if (j > d->samples.size())
            break;

        fft.perform(src, spectrum);
        for (int i = 0; i < NBins; ++i) {
            d->spectrum[i] = spectrum[i].r;
            d->spectrum2[i] = spectrum[i].i;
        }
        src += BinSize;

        qreal maxEnergy = 0;
        for (int i = 0; i < NBins; ++i) {
            if (spectrum[i].r > maxEnergy)
                maxEnergy = spectrum[i].r;
        }

        if (t0.elapsed() > 40) {
            d->percentReady = 100 * j / d->samples.size();
            update();
            t0.start();
        }
    }
    d->percentReady = 100;
    update();
}


void EnergyWidget::setSamples(const SampleBuffer &samples)
{
    Q_D(EnergyWidget);
    d->doCancel = false;
    d->samples = samples;
    d->analyzeFuture = QtConcurrent::run(this, &EnergyWidget::analyzeSamples);
}


bool EnergyWidget::isActive(void) const
{
    return d_ptr->analyzeFuture.isRunning();
}



void EnergyWidget::cancel(void)
{
    Q_D(EnergyWidget);
    d->doCancel = true;
    d->analyzeFuture.waitForFinished();
    d->spectrum.fill(0);
    d->spectrum2.fill(0);
    update();
}


void EnergyWidget::paintEvent(QPaintEvent*)
{
    Q_D(EnergyWidget);
    static const int xd = 4;
    qreal xs = qreal(width()) / (NBins - 1);
    qreal ys = qreal(height()) * 1e-6;
    QPainter p(this);
    p.fillRect(rect(), QColor(0x30, 0x20, 0x10));
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0xee, 0xcc, 0x33, 0xc0));
    QPainterPath path1;
    for (int i = 0; i < NBins / xd; ++i)
        path1.addRect(xd * (i - 1) * xs, height(), xd * xs, -qAbs(d->spectrum[i] * ys));
    p.drawPath(path1);
    p.setBrush(QColor(0xcc, 0x33, 0xee, 0xc0));
    QPainterPath path2;
    for (int i = 0; i < NBins / xd; ++i)
        path2.addRect(xd * (i - 1) * xs, 0, xd * xs, qAbs(d->spectrum2[i] * ys));
    p.drawPath(path2);
    if (d->analyzeFuture.isRunning()) {
        static const int padding = 2;
        p.setPen(Qt::white);
        p.drawText(QRect(padding, padding, width() - padding * 2, height() - padding * 2), Qt::AlignRight | Qt::AlignBottom, QString("%1%").arg(d->percentReady));
    }
}

