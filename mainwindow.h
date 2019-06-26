#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "multishipcontroller.h"
#include "multishipaisdata.h"
#include "routearray.h"
#include "usersdetails.h"
#include "userdatabaseclientconnection.h"
#include "amqpclient/shsclient/amqpserversettings.h"
#include "DbServerSettings/dbserversettings.h"
#include "userdatabase.h"
#include "sensorinterface.h"
#include "filteruserdata.h"
#include <QButtonGroup>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    MultiShipController *shipData;
    MultiShipAISData *aisData;
    RouteArray *routeArray;
    Communicator *_communicator;
    UsersDetails *userDetails;
    UserDatabase *userDatabase;
    SensorInterface *sensorInterface;
    FilterUserData *_filterUserData;

    QButtonGroup buttonGroup;
};

#endif // MAINWINDOW_H
