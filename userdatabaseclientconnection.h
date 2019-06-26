#ifndef USERDATABASECLIENTCONNECTION_H
#define USERDATABASECLIENTCONNECTION_H

#include <QDialog>

namespace Ui {
class UserDatabaseClientConnection;
}

class UserDatabaseClientConnection : public QDialog
{
    Q_OBJECT

public:
    explicit UserDatabaseClientConnection(QWidget *parent = 0);
    ~UserDatabaseClientConnection();

private:
    Ui::UserDatabaseClientConnection *ui;
};

#endif // USERDATABASECLIENTCONNECTION_H
