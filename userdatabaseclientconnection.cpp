#include "userdatabaseclientconnection.h"
#include "ui_userdatabaseclientconnection.h"

UserDatabaseClientConnection::UserDatabaseClientConnection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDatabaseClientConnection)
{
    ui->setupUi(this);
}

UserDatabaseClientConnection::~UserDatabaseClientConnection()
{
    delete ui;
}
