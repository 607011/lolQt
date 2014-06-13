// Copyright (c) 2014 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag.
// All rights reserved.

#include <QtCore/QDebug>
#include "consolewidget.h"
#include "ui_consolewidget.h"
#include "main.h"

ConsoleWidget::ConsoleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString("%1 - Console").arg(AppName));
}


ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}


void ConsoleWidget::clear(void)
{
    ui->consoleTextEdit->clear();
}


void ConsoleWidget::out(const QString &msg)
{
    ui->consoleTextEdit->append(msg);
}


void ConsoleWidget::err(const QString &msg)
{
    ui->consoleTextEdit->append(msg);
}
