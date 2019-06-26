#include "multishipcontroller.h"
#include "ui_multishipcontroller.h"

MultiShipController::MultiShipController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiShipController)
{
    ui->setupUi(this);
}

MultiShipController::~MultiShipController()
{
    delete ui;
}
