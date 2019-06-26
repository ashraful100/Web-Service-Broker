#include "vis.h"

VIS::VIS(QObject *parent) : QObject(parent)
{



    for(int i=1;i<=30;i++){

        mmsiMap[211000000+i] = i;


    }

    mmsiMap[333333333] = 31;


    mmsiMap [16401] =32;
    mmsiMap [16402] =33;
    mmsiMap [16403] =34;
    mmsiMap [16404] =35;
    mmsiMap [16405] =36;
    mmsiMap [16406] =37;
    mmsiMap [16407] =38;
    mmsiMap [16408] =39;
    mmsiMap [16409] =40;
    mmsiMap [16410] =41;
    mmsiMap [16411] =42;
    mmsiMap [16400] =43;
    mmsiMap [265590730] =44;


    QMapIterator<int, int> i(mmsiMap);
    while (i.hasNext()) {
        i.next();
        voyageMap[i.key()] =QString(createVoyage(i.key()));
    }

}

VIS::~VIS()
{

}

void VIS::respondToGetVoyage(int mmsi)
{
    if(mmsiMap.contains(mmsi)){

        QJsonObject mainObj;
        QJsonObject payloadObj;
        QJsonArray resultAry;
        QJsonObject route;
        route["route"]=voyageMap.value(mmsi);

        resultAry.append(route);
        payloadObj["result"] = resultAry;
        mainObj["payload"] = payloadObj;
        mainObj["mmsi"] =mmsi;
        mainObj ["message_type"] = "get_voyage_plan";
        mainObj ["request_message_type"] = "get_voyage_plan";
        QJsonDocument reqDoc(mainObj);
        QByteArray dataReq = reqDoc.toJson(QJsonDocument::Compact);
        //        qDebug() << "//////////////////**************/////////////////////";
        //        qDebug() <<dataReq;
        emit signalVoyageFromVis(QString(dataReq).simplified().toLocal8Bit());

    }
}

void VIS::slotRequestVoyage(QByteArray reqData)
{   int reqType = getRequestType(reqData);
    int mmsi = getMmsiFromVisReq(reqData);
    switch ((REQ_TYPE)reqType) {
    case VIS::REQ_INVALID:
        return;
        break;
    case VIS::REQ_POST_VOY:
        postVoyageLocallyAndRespond(reqData,mmsi);
        break;
    case VIS::REQ_GET_VOY:
        respondToGetVoyage(mmsi);
        break;
    case VIS::REQ_POST_MSG:
        break;

    }





    qDebug() << "Req MMSI" << mmsi;

}

void VIS::postVoyageLocallyAndRespond(QByteArray postVoyageMsg, int mmsi){

    voyageMap[mmsi] = parseVoyageFromRemote(postVoyageMsg);
    respondToGetVoyage(mmsi);
}

void VIS::slotPostVoyage(int mmsi)
{


    //qDebug() << "Req MMSI" << mmsi;
    if(mmsiMap.contains(mmsi)){

        QJsonObject mainObj;
        QJsonObject payloadObj;
        payloadObj["plan"] = QString(createVoyage(mmsi));
        mainObj["payload"] = payloadObj;
        mainObj["mmsi"] =mmsi;
        mainObj ["message_type"] = "post_voyage_plan";
        QJsonDocument reqDoc(mainObj);
        QByteArray dataReq = reqDoc.toJson(QJsonDocument::Compact);
        qDebug() <<mmsi<<"----------------------------------------------------";
        qDebug() <<dataReq;
        emit signalPostVoyageToVis(QString(dataReq).simplified().toLocal8Bit());

    }
}


int VIS::getRequestType(QByteArray visReq)
{

    QJsonParseError e;
    QJsonDocument  jsonDoc = QJsonDocument::fromJson(visReq,&e);
    if(e.error !=QJsonParseError::NoError){
        qDebug() << "Invalid Request";
        return REQ_INVALID;
    }
    QJsonObject docObject = jsonDoc.object();
    QString reqType =  docObject.value("message_type").toString();
    if(reqType.contains("post_voyage_plan"))
        return REQ_POST_VOY;

    if(reqType.contains("get_voyage_plan"))
        return REQ_GET_VOY;


}


int VIS::getMmsiFromVisReq(QByteArray visReq)
{
    qDebug() << visReq;
    QJsonParseError e;
    QJsonDocument  jsonDoc = QJsonDocument::fromJson(visReq,&e);
    if(e.error !=QJsonParseError::NoError){
        qDebug() << "Invalid Request";
        return -1;
    }
    QJsonObject docObject = jsonDoc.object();
    return docObject.value("mmsi").toInt();
}

QByteArray VIS::createVoyage(int mmsi)
{
    VoyageDetailsPtr voy =VoyageDetailsPtr(new VoyageDetails);
    QString from = "A";
    QString to = "B";
    
    int factor =mmsiMap.value(mmsi);
    
    voy->setAlternate(false);
    voy->setFrom(from);
    voy->setTo(to);
    voy->setName(QString("VOY_%1").arg(factor));
    voy->setShipName(QString("Ship_%1").arg(factor));
    voy->setUrn(QString("urn::stm::sc::cml::voy%1").arg(factor));
    QDateTime eta = QDateTime::currentDateTime();
    QList<WaypointPtr> wps;
    for(int i=0;i<4;i++){

        WaypointPtr wp(new Waypoint);
        double lat = 55+0.5*factor;
        double lon = 3+i+1;

        double dist = QGeoCoordinate(lat,lon).distanceTo(QGeoCoordinate(lat,lon+i));
        int sec = qAbs(dist/(10*0.5144));
        wp->setName(QString("WP_SHIP%1_NO%2").arg(factor).arg(i+1));
        wp->setCoordinate(QGeoCoordinate(lat,lon));
        wp->setSpeed(10);
        wp->setPortsideXTD(1);
        wp->setStarboardXTD(1);
        wp->setArrivalTime(eta.addSecs(sec));
        wp->setId(i+1);
        wps.append(wp);
    }
    voy->setWaypoints(wps);
    return parser.createXmlFromVoyage(voy);
}


void VIS::postToVIS(){
return;
    QMapIterator<int, int> i(mmsiMap);
    while (i.hasNext()) {
        i.next();
        slotPostVoyage(i.key());
    }
}

void VIS::postSingle(int mmsi){

    slotPostVoyage(mmsi);
}
QString VIS::parseVoyageFromRemote(QByteArray raw){
    //    qDebug() <<"*******************************************";
    //    qDebug() <<raw;
    //    qDebug() <<"*******************************************";
    QJsonParseError e;
    QJsonDocument  jsonDoc = QJsonDocument::fromJson(raw,&e);

    if(e.error !=QJsonParseError::NoError){
        qDebug()<< e.errorString();
        qDebug() << e.offset;
        qDebug() << "could not read Message!";
    }
    QJsonObject docObject = jsonDoc.object();

    int mmsi = docObject.value("mmsi").toInt();

    QString msgType = docObject.value("message_type").toString();
    QJsonObject payload = docObject.value("payload").toObject();
    if(msgType.contains("post_voyage_plan")){
        return getVoyageFromMessage(payload);
    }

}


QString VIS::getVoyageFromMessage(QJsonObject payload)
{
    QString rawVoyage = payload.value("plan").toString();


    qDebug() << "555555555555555555555555555555555";
    qDebug() <<rawVoyage;
    return rawVoyage;

}

