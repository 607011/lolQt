// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QPainter>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QtCore/QDebug>

#include "imagewidget.h"

class ImageWidgetPrivate
{
public:
    ImageWidgetPrivate()
        : windowAspectRatio(0)
        , imageAspectRatio(0)
    { /* ... */ }
    QImage image;
    QString imageFileName;
    QRect destRect;
    qreal windowAspectRatio;
    qreal imageAspectRatio;
};


ImageWidget::ImageWidget(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new ImageWidgetPrivate())
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}


void ImageWidget::setImage(const QImage& image)
{
    Q_D(ImageWidget);
    d->image = image.convertToFormat(QImage::Format_ARGB32);
    d->imageAspectRatio = (qreal)d->image.width() / d->image.height();
    update();
}

const QString &ImageWidget::imageFileName(void) const {
    return d_ptr->imageFileName;
}


void ImageWidget::resizeEvent(QResizeEvent* e)
{
    Q_D(ImageWidget);
    d->windowAspectRatio = (qreal)e->size().width() / e->size().height();
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    Q_D(ImageWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (d->image.isNull() || qFuzzyIsNull(d->imageAspectRatio) || qFuzzyIsNull(d->imageAspectRatio))
        return;
    if (d->windowAspectRatio < d->imageAspectRatio) {
        const int h = int(width() / d->imageAspectRatio);
        d->destRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * d->imageAspectRatio);
        d->destRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(d->destRect, d->image);
}


void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls() && d->urls().first().toString().contains(QRegExp("\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
        e->acceptProposedAction();
    else
        e->ignore();
}


void ImageWidget::dragLeaveEvent(QDragLeaveEvent* e)
{
    e->accept();
}


void ImageWidget::dropEvent(QDropEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls()) {
        QString fileUrl = d->urls().first().toString();
        if (fileUrl.contains(QRegExp("file://.*\\.gif$")))
#ifdef WIN32
            loadImage(fileUrl.remove("file:///"));
#else
            loadImage(fileUrl.remove("file://"));
#endif
    }
}


void ImageWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control) {
    }
}


void ImageWidget::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Control) {
    }
}


void ImageWidget::mouseMoveEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
}


void ImageWidget::mousePressEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
}


bool ImageWidget::loadImage(const QString& fileName)
{
    Q_D(ImageWidget);
    if (fileName.isEmpty()) {
        qWarning() << "ImageWidget::loadImage(): fileName is empty.";
        return false;
    }
    const QImage image(fileName);
    if (image.isNull())
        return false;
    setImage(image);
    d->imageFileName = fileName;
    emit gifDropped(fileName);
    return true;
}
