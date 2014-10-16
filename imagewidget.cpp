// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QPainter>
#include <QMimeData>
#include <QPixmap>
#include <QUrl>
#include <QtCore/QDebug>

#include "imagewidget.h"


class ImageWidgetPrivate
{
public:
    ImageWidgetPrivate(void) {}
};


ImageWidget::ImageWidget(QLabel *parent)
    : QLabel(parent)
    , d_ptr(new ImageWidgetPrivate())
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setSizePolicy(sizePolicy);
    setAcceptDrops(true);
    setPixmap(QPixmap(":/images/drop-icon.png"));
    setAlignment(Qt::AlignCenter);
}


void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls() && d->urls().first().toString().contains(QRegExp("\\.(png|gif|mp3|m4a)$")))
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
#ifdef WIN32
        fileUrl = fileUrl.remove("file:///");
#else
        fileUrl = fileUrl.remove("file://");
#endif
        if (fileUrl.endsWith((".gif")) || fileUrl.endsWith((".png"))) {
            loadImage(fileUrl);
        }
        else if (fileUrl.endsWith((".mp3")) || fileUrl.endsWith(".m4a")) {
            loadMusic(fileUrl);
        }
    }
}


void ImageWidget::loadImage(const QString& fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "ImageWidget::loadImage(): fileName is empty.";
        return;
    }
    emit gifDropped(fileName);
}


void ImageWidget::loadMusic(const QString& fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "ImageWidget::loadMusic(): fileName is empty.";
        return;
    }
    emit musicDropped(fileName);
}
