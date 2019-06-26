#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QObject>
#include <QSqlDatabase>
#include <QTime>
#include <QSqlQuery>
#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include <QByteArray>
#include <QMap>

class DatabaseSettings : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseSettings(QObject *parent = 0);
    ~DatabaseSettings();
    bool openDB();
    void closeDB();
    void importData();
    void storeData();
    void selectDB();
    void sensor1();
    void sensor2();
    void sensor3();
    void sensor4();


    QSqlDatabase db;

    bool opened=false;

signals:

public slots:


};

#endif // DATABASESETTINGS_H
