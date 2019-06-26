#include "shsdata.h"
#include <QTimeZone>
#include <QtMath>
//!
//! \brief This Modified SHS data class is to be specifically used for XML data  file generated for DSME simulator with the supplied Excel sheet containing the
//!  reference element IDs.
//! \param parent
//!
ShsData::ShsData(QObject *parent) :
    QObject(parent)
{
    engineId =2;
    shipId =1;
    rudderId = 0;


}
//!
//! \brief Receives the raw XML data decoded MAP from the server
//! \param shsData Data MAp containing decoded raw XML data (Both OS and TS data)
//!
void ShsData::onShsRead(QMap<QString, QString> shsData)
{
    setOwnShipData(shsData);
}
//!
//! \brief Filters the raw data map into ownship datamap
//! \param shsData contains raw SHS data
//!
void ShsData::setOwnShipData(QMap<QString, QString> shsData){


    ownship["id"]=QString::number(shipId);
    ownship["name"]=shsData["OS_Name_name_0"];

    // Change -/+180 to 0 to 360
    double hdg = shsData["OS_Heading_heading_0"].toDouble();
    if(hdg<0){
        hdg=360+hdg;
    }
    // Change -/+180 to 0 to 360
    double cog = shsData["OS_CourseOverGround_courseOverGround_0"].toDouble();
    if(cog<0){
        cog=360+cog;
    }
    ownship["actualName"]=shsData["OS_Name_name_0"];
    ownship["lat"]=shsData["OS_Latitude_latitude_0"];
    ownship["lon"]=shsData["OS_Longitude_longitude_0"];
    ownship["alt"]=shsData["OS_Altitude_altitude_0"];
    ownship["hdg"]=QString::number(hdg);
    ownship["loa"]=shsData["OS_Length_length_0"];
    ownship["boa"]=shsData["OS_Width_width_0"];
    ownship["sog_ms"]=QString::number(ShipSpeed);
    ownship["sog"]=QString::number(ShipSpeed*1.94384);
    ownship["cog"]=QString::number(cog);
    ownship["stw"]=QString::number(stw*1.94384);
    ownship["rpm"]=QString::number(rpm);
    ownship["rot"]=shsData["OS_RateOfTurn_rateOfTurn_0"];
    ownship["cmdEot"]=shsData[QString("OS_EOT_EOT_%1").arg(engineId)];
    ownship["cmdCourse"]="";
    ownship["simTime"]=shsData["SimulationTime"];
    ownship["cmdRudder"]=shsData[QString("OS_Rudder_rudder_%1").arg(rudderId)];
    ownship["windVel"] = shsData["OS_Wind_value_0"];
    ownship["windDir"] = shsData["OS_Wind_direction_0"];
    ownship["seaState"] = shsData["OS_SeaState_value_0"];
    ownship["seaStateDir"] = shsData["OS_SeaState_direction_0"];
    ownship["ukc"] = shsData["OS_EchoSounder_Value_echoSounder_Value_0"];
    ownship["windVel_sensor"] = shsData["OS_WindSensor_value_0"];
    ownship["windDir_sensor"] = shsData["OS_WindSensor_direction_0"];
    ownship["roll"] = shsData["OS_Roll_roll_0"];
    ownship["type"] = shsData["OS_TypeName_type_name_0"];
    ownship["callsign"] = shsData["OS_Callsign_callsign_0"];
    ownship["draught"] = shsData["OS_Draught_draught_0"];
    ownship["isSHS"] = QString::number(1);
    ownship["maptime"]=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

    // Emits os data map
    emit osObject(ownship);
    // For TS filtering
    setTrafShipData(shsData,ownship);

}
//!
//! \brief Filters the raw data map into trafficship datamap. A map is emitted for eaxh traffic ship.
//! \param shsData Data Map containing decoded raw XML data (Both OS and TS data)
//! \param osMap contains filetered ownshipdata for relative os ts data calculation
//!
void ShsData::setTrafShipData(QMap<QString, QString> shsData,QMap<QString, QString> osMap){

    int numOfTraffships = shsData["NumberOfTargetShips_numberOfTargetShips_0"].toInt();
    for(int i =0;i<numOfTraffships;i++){
        trafficship.clear();
        trafficship["id"] = QString::number(i);
        trafficship["osName"]=shsData["OS_Name_name_0"];
        trafficship["numOfTS"] = QString::number(numOfTraffships);
        trafficship["mmsi"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_ObjID_objId")];
        trafficship["altitude"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Altitude_altitude")];
        trafficship["callsign"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Callsign_callsign")];
        trafficship["cog"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_CourseOverGround_courseOverGround")];
        trafficship["draught"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Draught_draught")];
        trafficship["hdg"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Heading_heading")];
        trafficship["lat"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Latitude_latitude")];
        trafficship["lon"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Longitude_longitude")];
        trafficship["loa"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Length_length")];
        trafficship["boa"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Width_width")];
        trafficship["name"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Name_name")];
        trafficship["objtype"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_ObjectType_objectType")];
        trafficship["rot"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_RateOfTurn_rateOfTurn")];
        trafficship["maptime"]=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

        //u v vector to magnitude
        double sog_u = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedOverGround_u")].toDouble();
        double sog_v = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedOverGround_v")].toDouble();
        double sog = qSqrt(qPow(sog_u,2) +qPow(sog_v,2));
        trafficship["sog_ms"] = QString::number(sog,'f',2);
        trafficship["sog"] = QString::number(sog*1.94384,'f',2);

        // u v vector to magnitude
        double stw_u = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedThroughWater_u")].toDouble();
        double stw_v = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedThroughWater_v")].toDouble();
        double stw = qSqrt(qPow(stw_u,2) +qPow(stw_v,2));

        trafficship["stw_ms"] = QString::number(stw,'f',2);
        trafficship["stw"] = QString::number(stw*1.94384,'f',2);

        trafficship["type"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_TypeName_type_name")];

        // distance and bearing from ownship
        trafficship["distToOs"] =  QString::number(QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble()).distanceTo( QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble())));
        trafficship["brngToOs"] =  QString::number(QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble()).azimuthTo(QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble())));

        // Flag for SHS TS data to differentiate from AIS TS data
        trafficship["isSHS"] = QString::number(1);
        // Emit TS data map for each traffic ship
        emit tsObject(trafficship);
    }
}
//!
//! \brief Setter for Ship Identifier
//! \param shipId
//!
void ShsData::setShipId(int shipId)
{
    this->shipId =shipId;
}


