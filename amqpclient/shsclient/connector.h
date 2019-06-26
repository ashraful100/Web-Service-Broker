#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>
#include <QTimer>
#include <QMap>

namespace Ui {
class Connector;
}


struct OnlineShip {
    QString shipName;
    int available=0;
    QString server;
};

//!
//! \brief The Connector class establishes the connection between testbed and amqp queues
//! \ingroup shsclient
//!
class Connector : public QWidget
{
    Q_OBJECT

public:
    explicit Connector(QString AppId, int shipId, QWidget *parent = 0);
    ~Connector();


public slots:
    void setConnectionIdentifiers(int shipIndex, int userIndex);
    void osData(QMap<QString, QString> osData);
    void setAvailable(int shsId, int availabilityCode,QString &server, QString &name);

    void connectToShip(int shsIndex, int userIndex, bool connect);
signals:
    void signalOnConnectionActivated(bool connect,int ship,int user);
    void signalSelectedShipData(int shipId,int shsId,int connected,int availabilityCode,QString &server, QString &name);
private slots:


    void on_pbSettings_clicked(bool checked);

    void resetToShipSelector();
    void on_cbShip_activated(int index);

    void on_cbSystem_activated(int index);

    void on_pbConnect_clicked(bool checked);

    void on_pbConnect_toggled(bool checked);

    void populateShipList();
    bool compareData(OnlineShip first, OnlineShip second);
    void sendStatusInfo();
private:
    Ui::Connector *ui;

    int shipIndex,userIndex,shipId;
    QString shipName,appId;

    bool connectionStatus;
    QMap<int ,OnlineShip> availabilityMap;

    QStringList shipList;

};

#endif // CONNECTOR_H
