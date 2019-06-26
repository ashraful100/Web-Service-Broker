#ifndef FILTERUSERDATA_H
#define FILTERUSERDATA_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

class FilterUserData : public QObject
{
    Q_OBJECT
public:
    explicit FilterUserData(QObject *parent = nullptr);

signals:
    void sendDataToUserDetails(QByteArray);
    void sendDataToSensorInterface(QByteArray);
    void transmitterKey(QString);

public slots:
    void checkTypeOfData(QByteArray array);
};

#endif // FILTERUSERDATA_H
