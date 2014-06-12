// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __IMAGEWIDGET_H_
#define __IMAGEWIDGET_H_

#include <QWidget>
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

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget* parent = NULL);
    void setImage(const QImage&);
    const QString &imageFileName(void) const;
    bool loadImage(const QString& fileName);
    virtual QSize minimumSizeHint(void) const { return QSize(256, 256); }

signals:
    void gifDropped(const QString &fileName);

protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);

private:
    QScopedPointer<ImageWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ImageWidget)
    Q_DISABLE_COPY(ImageWidget)

};

#endif // __IMAGEWIDGET_H_
