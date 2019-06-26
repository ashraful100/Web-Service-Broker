#include "nmeatoxml.h"

NmeaToXml::NmeaToXml(QObject *parent) : QObject(parent)
{
    //connect(&_shsDataTemplate,SIGNAL(onShsDataUpdate(QString&,QString&)),this,SLOT(onShsDataUpdate(QString&,QString&)));
    xmlHeartBeat = "<?xml version='1.0' encoding='UTF-8'?>"
                   "<send>"
                   "<heartbeat>"
                   "<NumberOfOwnships>1</NumberOfOwnships>"
                   "<NumberOfTrafficShips>%1</NumberOfTrafficShips>"
                   "<NumberOfTugs>0</NumberOfTugs>"
                   "<SimulationRunningStatus>1</SimulationRunningStatus>"
                   "<SimulationTime>%2</SimulationTime>"
                   "</heartbeat>";

  osDataTemplate = _shsDataTemplate.getOsDataTemplate();
}

NmeaToXml::~NmeaToXml()
{

}


void NmeaToXml::setOsDataTemplate(const QString &value)
{
    osDataTemplate = value;
}


QString NmeaToXml::getOsData()
{

    QString osData;


    nmeaDataMap["loa"] = QString("200");
    nmeaDataMap["boa"] = QString("35");


    osData = osDataTemplate
            .arg(nmeaDataMap.value("name")) // Name
            .arg(nmeaDataMap.value("loa")) // Length
            .arg(nmeaDataMap.value("boa")) // Width
            .arg(nmeaDataMap.value("cog")) // Cog
            .arg(nmeaDataMap.value("lat")) // lat
            .arg(nmeaDataMap.value("lon")) // lon
            .arg(nmeaDataMap.value("sog")) // Sog
            .arg(nmeaDataMap.value("stwKn").toDouble()*0.5144) // Stw
            .arg(nmeaDataMap.value("rot")) // Rot
            .arg(nmeaDataMap.value("hdg")) // Hdg
            .arg(nmeaDataMap.value("eot")) // Eot //WHY MULTIPLYING BY 10 ??????????????????????????????????????????????????????????????
            .arg(nmeaDataMap.value("rpm")) // Rpm
            .arg(nmeaDataMap.value("cmdRudder")) // Rudder
            .arg(nmeaDataMap.value("windDir")) // Wind Dir
            .arg(nmeaDataMap.value("windVel")) // Wind Val
            .arg(nmeaDataMap.value("seaStateDir")) // Sea State Dir
            .arg(nmeaDataMap.value("seaStateVal")) // Sea state Val
            .arg(0); //Number Of Traffic Ships



    return osData;
}

QString NmeaToXml::getHeartBeat(){
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    qint64 timestampInMS =dateTime.toMSecsSinceEpoch()/1000;
    return xmlHeartBeat
            .arg(0)
            .arg(timestampInMS);
}


void NmeaToXml::sendShsData(){
    QString sendData  = QString("%1%2%3")
            .arg(getHeartBeat())
            .arg(getOsData())
            .arg("</send>");
    //qDebug()<< sendData;
    emit signalSendShsData(sendData.toLocal8Bit());
}


void NmeaToXml::setNmeaDataMap(QMap<QString, QString> &value)
{
    nmeaDataMap = value;
    sendShsData();
}


