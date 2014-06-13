// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QThread>
#include <QtCore/QDebug>

#include "wavewidget.h"
#include "ui_wavewidget.h"

class WaveWidgetPrivate {
public:
    QVector<qint16> samples;
};

WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WaveWidget)
    , d_ptr(new WaveWidgetPrivate)
{
    ui->setupUi(this);
    moveToThread(new QThread);
}


WaveWidget::~WaveWidget()
{
    delete ui;
}


void WaveWidget::append(const QAudioBuffer& buf)
{
    Q_D(WaveWidget);
    if (buf.format().sampleSize() == 16) {
        for (int i = 0; i < buf.sampleCount(); ++i)
            d->samples.append(buf.constData<qint16>()[i]);
    }
    update();
}


void WaveWidget::clear(void)
{
    Q_D(WaveWidget);
    d->samples.clear();
}


void WaveWidget::paintEvent(QPaintEvent*)
{
    Q_D(WaveWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    p.setBrush(Qt::transparent);
    p.setPen(Qt::green);
    QPointF p0(0, height() / 2);
    QPointF p1;
    int n = 0;
    for (QVector<qint16>::const_iterator i = d->samples.constBegin(); i != d->samples.constEnd(); ++i) {
        p1 = QPointF(width() * qreal(n) / d->samples.count(), height() / 2 * (1 + qreal(*i) / 32678));
        p.drawLine(p0, p1);
        p0 = p1;
        ++n;
    }
}
