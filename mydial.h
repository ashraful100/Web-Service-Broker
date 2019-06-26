#ifndef MYDIAL_H
#define MYDIAL_H
#include <QPainter>
#include <mainwindow.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class MyDial : public QwtDial
{
    Q_OBJECT

signals:
    void sendAngle(double data);
public:
    MyDial(QWidget *parent = NULL );    
    double val;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // MYDIAL_H
