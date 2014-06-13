// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#ifndef __CONSOLEWIDGET_H_
#define __CONSOLEWIDGET_H_

#include <QWidget>

namespace Ui {
class ConsoleWidget;
}

class ConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget *parent = 0);
    ~ConsoleWidget();

public slots:
    void clear(void);
    void out(const QString&);
    void err(const QString&);

private:
    Ui::ConsoleWidget *ui;
};

#endif // __CONSOLEWIDGET_H_
