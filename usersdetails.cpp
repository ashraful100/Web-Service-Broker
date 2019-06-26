#include "usersdetails.h"
#include "ui_usersdetails.h"

UsersDetails::UsersDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersDetails)
{
    ui->setupUi(this);
    data_reqId=0;
    data_userName=0;
}

UsersDetails::~UsersDetails()
{
    delete ui;
}

void UsersDetails::onMessageReceivedFromWeb(QByteArray userDetail)
{   
    qDebug()<<"Received data"<<userDetail;
    QJsonDocument document = QJsonDocument::fromJson(userDetail);

    UserInfo userInfo;
    userInfo.userId = document.object().value("userId").toString();
    userInfo.userName = document.object().value("userName").toString();
    userInfo.appId =  document.object().value("appId").toString();
    userInfo.reqId = document.object().value("reqId").toInt();

    if(userMap.contains( userInfo.userId))
    {
        userMap.insert(userInfo.userId, userInfo);
        int value = userIdToRowMap[userInfo.userId];
        QListWidgetItem *listWidgetItem = ui->usersName->item(value);
        UserData *object = qobject_cast<UserData *>(ui->usersName->itemWidget(listWidgetItem));
        object->setData(userInfo.reqId,userInfo.userId,userInfo.userName,userInfo.appId);
        ui->usersName->setItemWidget(listWidgetItem, object);
    }
    else
    {
        userMap.insert(userInfo.userId, userInfo);
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->usersName);
        ui->usersName->addItem(listWidgetItem);
        UserData *object = new UserData(this);
        object->setData(userInfo.reqId,userInfo.userId,userInfo.userName,userInfo.appId);
        listWidgetItem->setSizeHint(object->sizeHint());
        ui->usersName->setItemWidget(listWidgetItem, object);
        int rowNumber = ui->usersName->row(listWidgetItem);
        userIdToRowMap.insert(userInfo.userId,rowNumber);
    }
    emit sendUsersList(userMap);
}
//{"appId":"234","reqId":5,"userId":"5a154a20206bed14eca87405","userName":"Lennart"}

void UsersDetails::on_usersName_itemActivated(QListWidgetItem *item)
{

}
