#include "trafficsignal.h"
#include "ui_trafficsignal.h"
#include <QPainter>

TrafficSignal::TrafficSignal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrafficSignal)
{
    ui->setupUi(this);

    QPainter p(this);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
    p.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    p.drawEllipse(1, 1, width() - 1, height() - 1);
    p.end();
    ui->label->paintEvent(event);

}

TrafficSignal::~TrafficSignal()
{
    delete ui;
}
