// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __IMAGEWIDGET_H_
#define __IMAGEWIDGET_H_

#include <QLabel>
#include <QString>
#include <QRgb>
#include <QImage>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScopedPointer>


class ImageWidgetPrivate;

class ImageWidget : public QLabel
{
    Q_OBJECT

public:
    explicit ImageWidget(QLabel* parent = NULL);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

signals:
    void gifDropped(const QString &fileName);
    void musicDropped(const QString &fileName);

protected:
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

private:
    QScopedPointer<ImageWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ImageWidget)
    Q_DISABLE_COPY(ImageWidget)

private: // methods
    void loadImage(const QString& fileName);
    void loadMusic(const QString& fileName);


};

#endif // __IMAGEWIDGET_H_
