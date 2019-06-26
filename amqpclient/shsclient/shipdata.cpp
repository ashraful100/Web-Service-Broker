#include "shipdata.h"

ShipData::ShipData(QObject *parent) : QObject(parent)
{
    //    eotMapDefault[17.2]=100;
    //    eotMapDefault[12.4]=78;
    //    eotMapDefault[10.0]=70;
    //    eotMapDefault[7.8]=58;
    //    eotMapDefault[5.4]=41;
    //    eotMapDefault[0.0]=0;
    //    eotMapDefault[-5.2]=-40;
    //    eotMapDefault[-7.6]=-66;
    //    eotMapDefault[-11.0]=-100;

    //    eotMapDefault[25.5]=100;
    //    eotMapDefault[21]=78;
    //    eotMapDefault[18.5]=70;
    //    eotMapDefault[15.5]=58;
    //    eotMapDefault[10]=41;
    //    eotMapDefault[0.0]=0;
    //    eotMapDefault[-5.2]=-40;
    //    eotMapDefault[-7.6]=-66;
    //    eotMapDefault[-11.0]=-100;



    setDefaultShipName("King Darius");
    loadSettingsForShip(defaultShipName);

}

ShipData::~ShipData()
{

}

void ShipData::broadcastSpeedMaps(){

    emit signalSendSpeedMaps(eotMap,sogMap);
    emit signalSpeedAtMaxEot(sogMap.value(100));
}

void ShipData::readSettings(){

    sogMap.clear();
    eotMap.clear();
    QSettings shipDataSettings(qApp->applicationDirPath()+QString("/shipData.ini"),QSettings::IniFormat);

    shipDataSettings.beginGroup(QString("%1").arg(currentShipName));
    int size =  shipDataSettings.beginReadArray("EOT");
    if(size>0){
    for (int i = 0; i < size; ++i) {
        shipDataSettings.setArrayIndex(i);
        int eot = shipDataSettings.value("eot").toInt();
        double sog = shipDataSettings.value("sog").toDouble();
        sogMap[eot] =sog;
        eotMap[sog] =eot;
    }
    }
    else{
        loadSettingsForShip(getDefaultShipName());
    }

    shipDataSettings.endArray();
    shipDataSettings.endGroup();
//    qDebug() <<sogMap;
//    qDebug() <<eotMap;

    broadcastSpeedMaps();
}


void ShipData::writeSettings(){

    QSettings shipDataSettings(qApp->applicationDirPath()+QString("/shipData.ini"),QSettings::IniFormat);
    shipDataSettings.beginGroup("Default");
    shipDataSettings.setValue("defaultShipName",defaultShipName);
    shipDataSettings.setValue("currentShipName",currentShipName);
    shipDataSettings.endGroup();

    shipDataSettings.beginGroup(QString("%1").arg(currentShipName));
    shipDataSettings.beginWriteArray("EOT");

    QMapIterator<double, int> i(eotMapDefault);
    int counter = 0;
    while (i.hasNext()) {
        i.next();
        shipDataSettings.setArrayIndex(counter);
        shipDataSettings.setValue("eot",i.value());
        shipDataSettings.setValue("sog",i.key());
        counter++;

    }

    shipDataSettings.endArray();
    shipDataSettings.endGroup();


}
void ShipData::loadSettingsForShip(QString shipName){
    currentShipName = shipName;
    readSettings();
}
QString ShipData::getDefaultShipName() const
{
    QSettings shipDataSettings(qApp->applicationDirPath()+QString("/shipData.ini"),QSettings::IniFormat);
    shipDataSettings.beginGroup("Default");
    QString defaultShipName = shipDataSettings.value("defaultShipName").toString();
    shipDataSettings.endGroup();
    return defaultShipName;
}

void ShipData::setDefaultShipName(const QString &value)
{
    defaultShipName = value;
    QSettings shipDataSettings(qApp->applicationDirPath()+QString("/shipData.ini"),QSettings::IniFormat);
    shipDataSettings.beginGroup("Default");
    shipDataSettings.setValue("defaultShipName",defaultShipName);
    shipDataSettings.endGroup();

}


QMap<int, double> ShipData::getSogMap() const
{
    return sogMap;
}

void ShipData::setSogMap(const QMap<int, double> &value)
{
    sogMap = value;
}

QMap<double, int> ShipData::getEotMap() const
{
    return eotMap;
}

void ShipData::setEotMap(const QMap<double, int> &value)
{
    eotMap = value;
}

