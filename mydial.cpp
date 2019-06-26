#include "mydial.h"


MyDial::MyDial(QWidget *parent):QwtDial(parent)
{
//    myDial w;
    this->setScaleArc(0,360);
    this->setLineWidth(15);
    this->setScale(0,360);
//    this->setScaleMaxMajor(40);
    this->setScaleStepSize(15);
    this->setWrapping(true);
    this->setOrigin(270);

    QwtDialSimpleNeedle *nd = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, Qt::black, Qt::red);
    this->setNeedle(nd);
    this->setWindowTitle("Custom QwtDial");
    this->show();
}

void MyDial::paintEvent(QPaintEvent *event)
{
    QwtDial::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::black);
    QFont font;
    font.setPointSize(10);
    painter.setFont(font);
    QString text = QString("%1 degree").arg(value());
//    auto c = rect().center();
//    auto Size = painter.fontMetrics().size(Qt::TextSingleLine, text);
//    painter.drawText(QPointF(c.x() -Size.width()/2, c.y()+5*Size.height()), text);
    double val2=value();
    emit sendAngle(val2);
}

