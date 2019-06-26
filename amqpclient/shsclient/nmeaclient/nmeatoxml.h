#ifndef NMEATOXML_H
#define NMEATOXML_H

#include <QObject>
#include <QtCore>
#include <QDateTime>
#include <QString>
#include <QByteArray>

#include "amqpclient/shsclient/nmeaclient/shsdatatemplate.h"

class NmeaToXml : public QObject
{
    Q_OBJECT
public:
    explicit NmeaToXml(QObject *parent = 0);
    ~NmeaToXml();


    void setOsDataTemplate(const QString &value);

signals:
    void signalSendShsData(QByteArray shsData);

public slots:
     void setNmeaDataMap(QMap<QString, QString> &value);
private slots:
    QString getHeartBeat();
    QString getOsData();
    void sendShsData();
private:
     QString osDataTemplate;
     ShsDataTemplate _shsDataTemplate;
     QString xmlHeartBeat;
     QMap<QString,QString> nmeaDataMap;
};

#endif // NMEATOXML_H
