#include "userdata.h"
#include "ui_userdata.h"
#include <QDebug>
UserData::UserData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserData)
{
    ui->setupUi(this);
}

void UserData::setData(int reqId, QString userId, QString userName, QString appId)
{
    ui->lbUserId->setText(userId);
    ui->lbUserName->setText(userName);   
    ui->leAppId->setText(appId);
    ui->leReqId->setText(QString::number(reqId));
}

UserData::~UserData()
{
    delete ui;
}

