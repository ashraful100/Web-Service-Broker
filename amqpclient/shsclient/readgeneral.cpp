
#include "readgeneral.h"

#include <QGeoCoordinate>
#include <QTime>
#include <QDebug>
#include <QObject>
#include <QSettings>



ReadGeneral::ReadGeneral(QObject *parent) :
    QObject(parent)
{


    _nmeaToXml =new NmeaToXml(this);

    connect(this,SIGNAL(signalSendNmeaDataMap(QMap<QString,QString>&)),_nmeaToXml,SLOT(setNmeaDataMap(QMap<QString,QString>&)));
    connect(_nmeaToXml,SIGNAL(signalSendShsData(QByteArray)),this,SIGNAL(signalXmlFromNmea(QByteArray)));
}

ReadGeneral::~ReadGeneral()
{

}

void ReadGeneral::slotSetOsAisdata(QMap<QString, QString> osAisDataMap)
{
    nmeaDataMap["name"] = osAisDataMap.value("name");
}

//on port read
void ReadGeneral::onDataReceived(QByteArray readDataAry)
{



    QString readDataStr(readDataAry);
    readDataStr =readDataStr.simplified();


    //Split the comma seperated NMEA message and store it as a list
    QStringList readDataList = readDataStr.split(",");
    QString readMsgId = readDataList.value(0);

    //msg identifier
    QString rotId = "ROT";
    QString rpmId = "RPM";
    QString gllId = "GLL";
    QString dbtId = "DBT";
    QString rsaId = "RSA";
    QString ggaId = "GGA";
    QString hdgId = "HDG";
    QString zdaId = "ZDA";
    QString vhwId = "VHW";
    QString mwvId = "MWV";
    QString dtmId = "DTM";
    QString hdtId = "HDT";

    // Identify the nmea msg based on identifier

    if(readMsgId.contains(rotId)){
        double rot = readDataList.value(1).toDouble();
        nmeaDataMap["rot"]= QString::number(rot);


    }   else if(readMsgId.contains(rpmId)){

        QString engineSrc=readDataList.value(1);
        int engineNum =readDataList.value(2).toInt();
        double rpm =readDataList.value(3).toDouble();
        double pitch =readDataList.value(4).toDouble();

        nmeaDataMap["rpm"]= QString::number(rpm);


    }   else if(readMsgId.contains(gllId)){

        QString gllTimeStr = readDataList.value(5);
        QTime gllTime = QTime::fromString(gllTimeStr,"hhmmss.00");
        gllLocation = parseLocationFromNmea(readDataList);
        gllNmeaMsg = readDataStr;

        nmeaDataMap["lat"] = QString::number(gllLocation.latitude());
        nmeaDataMap["lon"] = QString::number(gllLocation.longitude());
        //emit onReadGLLSignal(gllLocation, gllTime);
        //emit onReadGLLSignal(gllLocation);
        if(osDataSource==2)
            emit signalSendNmeaDataMap(nmeaDataMap);

    }   else if(readMsgId.contains(dbtId)){

        //emit onReadDbtSignal(readDataList.value(1).toDouble(),readDataList.value(3).toDouble(),readDataList.value(5).toDouble());
        nmeaDataMap["dbt"] =QString::number(readDataList.value(3).toDouble());
    }   else if(readMsgId.contains(rsaId)){
        nmeaDataMap["rudder_s"]  = readDataList.value(1);
        nmeaDataMap["rudder_p"]  = readDataList.value(3);
        //emit onReadRSASignal(readDataList.value(1).toDouble(),readDataList.value(3).toDouble());
        //emit onReadRSASignal(readDataList.value(1).toDouble());



    }   else if(readMsgId.contains(ggaId)){

        QString ggaTimeStr = readDataList.value(1);
        QTime ggaTime = QTime::fromString(ggaTimeStr,"hhmmss.00");
        QGeoCoordinate ggaLoc = parseLocationFromNmea(readDataList);
        //emit onReadGGASignal(ggaLoc, ggaTime);
        nmeaDataMap["ggaLat"] = QString::number(ggaLoc.latitude());
        nmeaDataMap["ggaLon"] = QString::number(ggaLoc.longitude());
        ggaNmeaMsg = readDataStr;

    }   else if(readMsgId.contains(hdgId)){

        heading = readDataList.value(1).toDouble();
        hdgNmeaMsg = readDataStr;
        //emit onReadHDGSignal(heading);

        nmeaDataMap["hdg"] = readDataList.value(1);
        nmeaDataMap["cog"] = readDataList.value(1);

    }  else if (readMsgId.contains(zdaId)) {
        QString zdaTimeStr = readDataList.value(1);
        QTime zdaTime = QTime::fromString(zdaTimeStr,"hhmmss.00");

        QString timeStamp = QString("%1%2%3")
                .arg(readDataList.value(2))//dd
                .arg(readDataList.value(3))//MM
                .arg(readDataList.value(4));//yyyy


        QDateTime shipTimeLocal = QDateTime::fromString(timeStamp,"ddMMyyyy");
        shipTimeLocal.setTime(zdaTime);
        qint64 shipTimeLocalMSecSinceEPOC =shipTimeLocal.toMSecsSinceEpoch();
        nmeaDataMap["simTime"] = QString::number(shipTimeLocalMSecSinceEPOC);
    }  else if (readMsgId.contains(vhwId)) {
        nmeaDataMap["stwKn"] = readDataList.value(5);
        nmeaDataMap["stwKmph"] = readDataList.value(6);

    } else if (readMsgId.contains(mwvId)) {
        nmeaDataMap["windVel"] = readDataList.value(3);
        nmeaDataMap["windVelUnit"] = readDataList.value(4); //K kmph | M m/s | N knots
        nmeaDataMap["windDir"] = readDataList.value(1);
        nmeaDataMap["windRef"] = readDataList.value(2); // R Relative | T True
    }else if (readMsgId.contains(dtmId)) {
        nmeaDataMap["refDatum"] = readDataList.value(8);
        nmeaDataMap["localDatum"] = readDataList.value(1);

    }else if (readMsgId.contains(hdtId)) {
        nmeaDataMap["hdgTrue"] = readDataList.value(1);

    }

    else{

        return;

    }

}


double ReadGeneral::getHeading()
{
    return heading;
}

QGeoCoordinate ReadGeneral::getOsLoc()
{
    return gllLocation;
}

QString ReadGeneral::getGllNmeaMsg()
{
    return gllNmeaMsg;
}

QString ReadGeneral::getGgaNmeaMsg()
{
    return ggaNmeaMsg;
}

QString ReadGeneral::getHdgNmeaMsg()
{
    return hdgNmeaMsg;
}

QGeoCoordinate ReadGeneral::parseLocationFromNmea(QStringList listNmeaLocation)
{
    //parse latitude
    QString latStr = listNmeaLocation.value(1);
    int point_index = latStr.indexOf(".");
    QString latMin = latStr.mid((point_index-2),6);
    QString latDeg = latStr.mid(0,(point_index-2));

    int latHem;
    if(listNmeaLocation.value(2).contains("S"))
    {
        latHem = -1;
    }
    else
    {
        latHem =1;
    }

    double latitude = latHem*(latDeg.toDouble()+latMin.toDouble()/60);

    //parse Longitude
    QString lonStr = listNmeaLocation.value(3);
    point_index = lonStr.indexOf(".");
    QString lonMin = lonStr.mid((point_index-2),6);
    QString lonDeg = lonStr.mid(0,(point_index-2));

    int lonHem;
    if(listNmeaLocation.value(4).contains("W",Qt::CaseInsensitive))
    {
        lonHem = -1;
    }
    else
    {
        lonHem =1;
    }

    double longitude = lonHem*(lonDeg.toDouble()+lonMin.toDouble()/60);

    //Update geo-location
    QGeoCoordinate loc;
    loc.setLatitude(latitude);
    loc.setLongitude(longitude);
    return loc;

}


void ReadGeneral::setOsDataSource(int value)
{
    osDataSource = value;
}


