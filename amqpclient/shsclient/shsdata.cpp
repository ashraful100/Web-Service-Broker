#include "shsdata.h"
#include <QTimeZone>
#include <QtMath>

ShsData::ShsData(QObject *parent) :
    QObject(parent)
{
    engineId =2;
    shipId =1;
    rudderId = 0;

    dataInterval =0;
    intervalcounter =0;
    scenarioRandomizerActive =false;

}

void ShsData::onShsRead(QMap<QString, QString> shsData)
{
    setOwnShipData(shsData);
}


void ShsData::setOwnShipData(QMap<QString, QString> shsData){


    QGeoCoordinate cur_loc;
    // double rpm = shsData["OS_PeripheralDevices_PropUnitsDynData_RPM_0"].toDouble();

    double rpm = shsData[QString("OS_CommandCombiner_RPM_commandCombiner_RPM_0")].toDouble();

    double ShipVelu = shsData.value("OS_SpeedOverGround_u_0").toDouble();
    double ShipVelv = shsData.value("OS_SpeedOverGround_v_0").toDouble();
    double ShipSpeed = sqrt(ShipVelu*ShipVelu+ShipVelv*ShipVelv);



    double stw_u = shsData.value("OS_SpeedThroughWater_u_0").toDouble();
    double stw_v = shsData.value("OS_SpeedThroughWater_v_0").toDouble();
    double stw = sqrt(stw_u*stw_u+stw_v*stw_v);


    cur_loc.setLatitude(shsData["OS_Latitude_latitude_0"].toDouble());
    cur_loc.setLongitude(shsData["OS_Longitude_longitude_0"].toDouble());

//    emit SimulationTime(shsData["SimulationTime"].toInt());
//    emit osRpm(shsData["OS_PeripheralDevices_PropUnitsDynData_RPM_0"].toDouble());
//    emit osCog(shsData["OS_CourseOverGround_courseOverGround_0"].toDouble());
//    emit osSog(ShipSpeed*1.94384);
//    emit ownShipData(shsData["OS_Heading_heading_0"].toDouble(),cur_loc,shsData["OS_Length_length_0"].toDouble(),shsData["OS_Width_width_0"].toDouble(),shsData["OS_Name_name_0"],shipId);
//    emit commandCombinerEot(shsData[QString("OS_CommandCombiner_EOT_commandCombiner_EOT_%1").arg(engineId)].toDouble());
//    emit shipName(shsData["OS_Name_name_0"]);

//    emit localTime(QTime::currentTime());
    ownship["ship"]="os";
    ownship["id"]=QString::number(-1*shipId);
    ownship["mapId"]=QString::number(-1*shipId);
    ownship["name"]=virtualShipName;
    if(scenarioRandomizerActive){

        ownship["name"]=virtualShipName;
    }
    else
    {
        ownship["name"]=shsData["OS_Name_name_0"];
    }

//    emit shipNameWithId(ownship["name"],shipId);
    double hdg = shsData["OS_Heading_heading_0"].toDouble();
    if(hdg<0){
        hdg=360+hdg;
    }

    double cog = shsData["OS_CourseOverGround_courseOverGround_0"].toDouble();
    if(cog<0){
        cog=360+cog;
    }
    ownship["simulator"] =shsData["Simulator"];
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
    ownship["cmdEot"]=shsData[QString("OS_CommandCombiner_EOT_commandCombiner_EOT_%1").arg(engineId)];
    ownship["cmdCourse"]="";
    ownship["simTime"]=shsData["SimulationTime"];
    ownship["cmdRudder"]=shsData[QString("OS_CommandCombiner_Rudder_commandCombiner_Rudder_%1").arg(rudderId)];

    if( ownship["simulator"].contains("DSME"))
    {
        ownship["windVel"] = shsData["OS_Wind_value_0"];
        ownship["windDir"] = shsData["OS_Wind_direction_0"];
        ownship["seaState"] = "0";
        ownship["seaStateDir"] = shsData["OS_Wave_direction_0"];
        ownship["heave"] = shsData["OS_Heave_heave_0"];
    }
    else
    {
        ownship["windVel"] = shsData["EnvWind_Value_0"];
        ownship["windDir"] = shsData["EnvWind_direction_0"];
        ownship["seaState"] = shsData["EnvSeaState_Value_0"];
        ownship["seaStateDir"] = shsData["EnvSeaState_direction_0"];
        ownship["heave"] = shsData["OS_Altitude_altitude_0"];
    }


    ownship["ukc"] = shsData["OS_Sounding_Value_Displayed_0"];

    ownship["roll"] = shsData["OS_Roll_roll_0"];
    ownship["type"] = shsData["OS_TypeName_type_name_0"];
    ownship["callsign"] = shsData["OS_Callsign_callsign_0"];
    ownship["draught"] = shsData["OS_Draught_draught_0"];
    ownship["isSHS"] = QString::number(1);
    ownship["maptime"]=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

    //additional Data needed for HWC Sensor based assessment

    ownship["pitch"] = shsData["OS_Pitch_pitch_0"];
    ownship["wave_dir"] =shsData["OS_Wave_direction_0"];
    ownship["wave_height"] =shsData["OS_Wave_height_0"];
    ownship["wave_period"] =shsData["OS_Wave_period_0"];

    ownship["current_dir"] =shsData["OS_Current_direction_0"];
    ownship["current_speed"] =shsData["OS_Current_speed_0"];


    ownship["dataSource"] = "XML";
    int numOfTraffships = shsData["NumberOfTargetShips_numberOfTargetShips_0"].toInt();
    ownship["numOfTS"] = QString::number(numOfTraffships);
    emit osObject(ownship);

    //qDebug() << "Test TS XML";
    setTrafShipData(shsData,ownship);

}

void ShsData::setTrafShipData(QMap<QString, QString> shsData,QMap<QString, QString> osMap){

    int numOfTraffships = shsData["NumberOfTargetShips_numberOfTargetShips_0"].toInt();
    for(int i =0;i<numOfTraffships;i++){
        trafficship.clear();
        trafficship["ship"]="ts";
        trafficship["id"] = QString::number(i);
        trafficship["mapId"] = QString::number(i);
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
        trafficship["name"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Name_name")];

        //        qDebug()<<"---------------------------------"<< trafficship["name"];
        //        qDebug()<< QGeoCoordinate(trafficship["lat"].toDouble(),trafficship["lon"].toDouble()).toString(QGeoCoordinate::Degrees);
        //        qDebug()<< QGeoCoordinate(trafficship["lat"].toDouble(),trafficship["lon"].toDouble()).toString(QGeoCoordinate::DegreesMinutes);
        //        qDebug()<< QGeoCoordinate(trafficship["lat"].toDouble(),trafficship["lon"].toDouble()).toString(QGeoCoordinate::DegreesMinutesSeconds);
        //        qDebug()<<"---------------------------------";
        trafficship["loa"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Length_length")];
        trafficship["boa"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_Width_width")];


        //        QMap::value("TRS_0_TS_SpeedOverGround_u");
        //        QMap::value("TRS_0_TS_SpeedOverGround_v");
        //        QMap::value("TRS_0_TS_SpeedOverGround_w");




        // qDebug() <<  trafficship["name"] << trafficship["boa"] << trafficship["loa"] ;
        trafficship["objtype"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_ObjectType_objectType")];
        trafficship["rot"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_RateOfTurn_rateOfTurn")];
        trafficship["maptime"]=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

        double sog_u = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedOverGround_u")].toDouble();
        double sog_v = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedOverGround_v")].toDouble();
        double sog = qSqrt(qPow(sog_u,2) +qPow(sog_v,2));
        trafficship["sog_ms"] = QString::number(sog,'f',2);
        trafficship["sog"] = QString::number(sog*1.94384,'f',2);


        double stw_u = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedThroughWater_u")].toDouble();
        double stw_v = shsData[QString("TRS_%1_%2").arg(i).arg("TS_SpeedThroughWater_v")].toDouble();
        double stw = qSqrt(qPow(stw_u,2) +qPow(stw_v,2));

        trafficship["stw_ms"] = QString::number(stw,'f',2);
        trafficship["stw"] = QString::number(stw*1.94384,'f',2);

        trafficship["type"] = shsData[QString("TRS_%1_%2").arg(i).arg("TS_TypeName_type_name")];

        //        trafficship["cpa"] = QString::number(_cpaCalc.calculateCPA(osMap.value("cog").toDouble(),
        //                                                                   osMap.value("sog").toDouble(),
        //                                                                   QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble()),
        //                                                                   trafficship.value("cog").toDouble(),
        //                                                                   trafficship.value("sog").toDouble(),
        //                                                                   QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble())
        //                                                                   ));

        //        trafficship["tcpa"] = QString::number(_cpaCalc.calculateTCPA(osMap.value("cog").toDouble(),
        //                                                                     osMap.value("sog").toDouble(),
        //                                                                     QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble()),
        //                                                                     trafficship.value("cog").toDouble(),
        //                                                                     trafficship.value("sog").toDouble(),
        //                                                                     QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble())
        //                                                                     ));
        trafficship["distToOs"] =  QString::number(QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble()).distanceTo( QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble())));
        trafficship["brngToOs"] =  QString::number(QGeoCoordinate(osMap.value("lat").toDouble(),osMap.value("lon").toDouble()).azimuthTo(QGeoCoordinate(trafficship.value("lat").toDouble(),trafficship.value("lon").toDouble())));
        trafficship["dataSource"] = "XML";

        trafficship["isSHS"] = QString::number(1);
        //qDebug() << "test ShsData" <<i;
        emit tsObject(trafficship);
    }
}

void ShsData::setShipId(int shipId)
{
    this->shipId =shipId;
}


void ShsData::setVirtualShipName(QString &virtualName)
{
    virtualShipName = virtualName;
}


void ShsData::setScenarioRandomiserState(bool state)
{
    scenarioRandomizerActive =state;
}
