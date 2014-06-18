// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QtConcurrent>
#include <QtCore/QDebug>

#include "wavewidget.h"

class WaveWidgetPrivate {
public:
    WaveWidgetPrivate(void)
        : waveForm(16 * 1024, 128, QImage::Format_RGB32)
        , timerId(0)
        , backgroundColor(0x30, 0x30, 0x30)
        , cancelDraw(false)
        , duration(0)
        , position(0)
    {
        waveForm.fill(backgroundColor);
    }
    SampleBuffer samples;
    QImage waveForm;
    int timerId;
    const QColor backgroundColor;
    QFuture<void> drawFuture;
    bool cancelDraw;
    qint64 duration;
    qint64 position;
    QMutex drawMutex;
};


WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new WaveWidgetPrivate)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
}


WaveWidget::~WaveWidget()
{
    // ...
}


void WaveWidget::drawWaveForm(void)
{
    Q_D(WaveWidget);
    QElapsedTimer t;
    t.start();
    d->drawMutex.lock();
    QPainter p(&d->waveForm);
    const qreal halfHeight = 0.5 * d->waveForm.height();
    const qreal xs = qreal(d->waveForm.width()) / qreal(d->samples.size());
    const qreal ys = qreal(halfHeight) / (2 << (8 * sizeof(SampleBufferType) - 2));
    p.fillRect(d->waveForm.rect(), d->backgroundColor);
    d->drawMutex.unlock();
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::transparent);
    p.setPen(QPen(QBrush(QColor(0x33, 0xcc, 0x44)), 0.1));
    static const int SampleStep = 4096;
    for (int i = 0; i < d->samples.size(); i += SampleStep) {
        if (d->cancelDraw)
            break;
        else {
            QMutexLocker(&d->drawMutex);
            const int EndStep = qMin(SampleStep + i, d->samples.size());
            for (int j = i; j < EndStep; ++j)
                p.drawPoint(QPointF(j * xs, halfHeight + ys * d->samples.at(j)));
        }
    }
    d->samples.clear();
    update();
    emit analysisCompleted();
    qDebug() << t.elapsed();
}



void WaveWidget::setSamples(const SampleBuffer &samples, qint64 duration)
{
    Q_D(WaveWidget);
    d->drawMutex.lock();
    d->cancelDraw = false;
    d->drawMutex.unlock();
    d->duration = duration;
    d->samples = samples;
    d->timerId = startTimer(40);
    d->drawFuture = QtConcurrent::run(this, &WaveWidget::drawWaveForm);
}


bool WaveWidget::isActive(void) const
{
    return d_ptr->drawFuture.isRunning();
}


void WaveWidget::cancel(void)
{
    Q_D(WaveWidget);
    d->cancelDraw = true;
    killTimer(d->timerId);
    d->timerId = 0;
    d->drawFuture.waitForFinished();
    d->waveForm.fill(d->backgroundColor);
    update();
}


void WaveWidget::setPosition(qint64 position)
{
    Q_D(WaveWidget);
    d->position = position;
    update();
}


void WaveWidget::setDuration(qint64 duration)
{
    Q_D(WaveWidget);
    d->duration = duration;
    update();
}


void WaveWidget::timerEvent(QTimerEvent *e)
{
    Q_D(WaveWidget);
    if (e->timerId() == d->timerId) {
        update();
        if (d->samples.size() == 0) {
            killTimer(d->timerId);
            d->timerId = 0;
        }
    }
}


void WaveWidget::paintEvent(QPaintEvent*)
{
    Q_D(WaveWidget);
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    d->drawMutex.lock();
    p.drawImage(rect(), d->waveForm);
    d->drawMutex.unlock();
    if (d->drawFuture.isRunning())
        p.fillRect(rect(), QColor(0x80, 0x80, 0x80, 0x80));
    if (d->position > 0 && d->duration > 0) {
        p.setPen(QColor(0xff, 0x22, 0x33));
        int x = width() * d->position / d->duration;
        p.drawLine(QPoint(x, 0), QPoint(x, height()));
    }
}
