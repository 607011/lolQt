// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QImage>
#include <QtConcurrent>
#include <QtCore/QDebug>

#include "wavewidget.h"
#include "ui_wavewidget.h"


class WaveWidgetPrivate {
public:
    WaveWidgetPrivate(void)
        : waveForm(16 * 1024, 128, QImage::Format_RGB32)
        , timerId(0)
        , backgroundColor(0x30, 0x30, 0x30)
    {
        QPainter p(&waveForm);
        p.fillRect(waveForm.rect(), backgroundColor);
    }
    QVector<qint16> samples;
    QImage waveForm;
    int timerId;
    const QColor backgroundColor;
};


WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new WaveWidgetPrivate)
{
}


WaveWidget::~WaveWidget()
{
    // ...
}


void WaveWidget::append(const QAudioBuffer &buf)
{
    Q_D(WaveWidget);
    if (buf.format().sampleSize() == 16) {
        for (int i = 0; i < buf.sampleCount(); ++i)
            d->samples.append(buf.constData<qint16>()[i]);
    }
}


void WaveWidget::clear(void)
{
    Q_D(WaveWidget);
    d->samples.clear();
}


void WaveWidget::drawWaveForm(void)
{
    Q_D(WaveWidget);
    QPainter p(&d->waveForm);
    p.fillRect(d->waveForm.rect(), d->backgroundColor);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::transparent);
    p.setPen(QPen(QBrush(Qt::green), 0.1));
    const qreal halfHeight = 0.5 * d->waveForm.height();
    QPointF p0(0, halfHeight);
    QPointF p1;
    const qreal xs = qreal(d->waveForm.width()) / qreal(d->samples.count());
    for (int x = 0; x < d->samples.count(); ++x) {
        p1 = QPointF(x * xs, halfHeight * (1 + qreal(d->samples.at(x)) / 32678));
        p.drawLine(p0, p1);
        p0 = p1;
    }
    update();
    clear();
}



void WaveWidget::finish(void)
{
    Q_D(WaveWidget);
    d->timerId = startTimer(20);
    QFuture<void> future = QtConcurrent::run(this, &WaveWidget::drawWaveForm);
    Q_UNUSED(future);
}


void WaveWidget::timerEvent(QTimerEvent*e)
{
    Q_D(WaveWidget);
    if (e->timerId() == d->timerId) {
        update();
        if (d->samples.count() == 0)
            killTimer(d->timerId);
    }
}


void WaveWidget::paintEvent(QPaintEvent*)
{
    Q_D(WaveWidget);
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.drawImage(rect(), d->waveForm);
}
