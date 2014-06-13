// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __WAVEWIDGET_H_
#define __WAVEWIDGET_H_

#include <QWidget>
#include <QPaintEvent>
#include <QAudioBuffer>
#include <QScopedPointer>

namespace Ui {
class WaveWidget;
}

class WaveWidgetPrivate;

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveWidget(QWidget *parent = 0);
    ~WaveWidget();
    virtual QSize sizeHint(void) const { return QSize(128, 16); }

public slots:
    void append(const QAudioBuffer&);
    void clear(void);

protected:
    void paintEvent(QPaintEvent*);

private:
    Ui::WaveWidget *ui;

    QScopedPointer<WaveWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(WaveWidget)
    Q_DISABLE_COPY(WaveWidget)

};

#endif // __WAVEWIDGET_H_
