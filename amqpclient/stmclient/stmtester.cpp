#include "stmtester.h"
#include "ui_stmtester.h"

STMTester::STMTester(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::STMTester)
{
    ui->setupUi(this);
}

STMTester::~STMTester()
{
    delete ui;
}

void STMTester::on_pbSub_clicked()
{
   emit signalSub(ui->leMMSI->text().toInt());
}

void STMTester::on_pbPost_clicked()
{
    emit signalPost(ui->leMMSI->text().toInt());
}

void STMTester::on_pbGet_clicked()
{
    emit signalGet(ui->leMMSI->text().toInt());
}

void STMTester::on_pbGetMsg_clicked()
{
     emit signalGetMsg(ui->leMMSI->text().toInt());
}
