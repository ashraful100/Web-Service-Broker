#include "filteruserdata.h"

FilterUserData::FilterUserData(QObject *parent) : QObject(parent)
{

}

void FilterUserData::checkTypeOfData(QByteArray array)
{
    qDebug()<<"FilterUserData received UserDetails: "<<array;
    QJsonDocument doc = QJsonDocument::fromJson(array);

    QString _appId = doc.object().value("appId").toString();
    int _dataId = doc.object().value("dataId").toInt();
    qDebug()<<_dataId;

    if(_dataId==0)
    {        
        qDebug()<<_dataId<<"JSON sent to UserDetails";
        emit sendDataToUserDetails(array);
        if(_appId=="MUH")
        {
//            QString key = QString("K_%1").arg(doc.object().value("userId").toString());
//            emit transmitterKey(key);
//            qDebug()<<"Emitted transmitter key"<<key;

            qDebug()<<_dataId<<"JSON sent to SensorInterface";
            emit sendDataToSensorInterface(array);

        }
    }
    else
    {
        qDebug()<<_dataId<<"JSON sent to SensorInterface";
        emit sendDataToSensorInterface(array);
    }
}
