#include "databaseinterface.h"




DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{

}

DatabaseInterface::~DatabaseInterface()
{
    closeDB();
}

int DatabaseInterface::storeNewSimulationInstance(QDateTime startTime, QString shortName, QString description)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO SimulationInstance (start_datetime, short_name, description) VALUES (?, ?, ?)");
    query.bindValue(0, startTime);
    query.bindValue(1, shortName);
    query.bindValue(2, description);

    query.exec();
    db.commit();

    QVariant lastInserted = query.lastInsertId();
    if(lastInserted.isValid())
        return lastInserted.toInt();
    else
        return -1;
}

void DatabaseInterface::storeEndTimeOfSimulationInstance(int simulationId, QDateTime endTime,bool success)
{
    QSqlQuery query(db);

    query.prepare("UPDATE SimulationInstance SET end_datetime=?,successful=? WHERE id=?");
    query.bindValue(0,endTime);
    query.bindValue(1,success);
    query.bindValue(2,simulationId);

    if(query.exec()){
        emit signalRecordingFinished(true,"Recording Stopped");
    } else{
       emit signalRecordingFinished(false,query.lastError().text());
    }
}


bool DatabaseInterface::storeShipMotionData(ShipMotionEntity *shipEntity, QDateTime simulationStartTime)
{
    qint64 millisecondsDiff = shipEntity->getTimeStamp() - simulationStartTime.toMSecsSinceEpoch();
    int timeStep = millisecondsDiff/1000;

    QSqlQuery query(db), query2(db);
    int siteUniqueId, appUniqueId, entityUniqueId;

    query.prepare("SELECT id FROM Site WHERE site_id="+ QString::number(shipEntity->getSiteId()));
    query.exec();
    if (query.next()) {
        siteUniqueId = query.value(0).toInt();
    } else {
        query2.prepare("INSERT INTO Site (site_id, date_created) VALUES (?, ?)");
        query2.bindValue(0, shipEntity->getSiteId());
        query2.bindValue(1, QDateTime::currentDateTime());

        query2.exec();
        db.commit();

        QVariant lastInserted = query.lastInsertId();
        if(lastInserted.isValid())
            siteUniqueId = lastInserted.toInt();
        else
            siteUniqueId = -1;
    }
//    query.prepare("SELECT id FROM Application WHERE application_id=" + QString::number(shipEntity->getApplicationId())+" AND site_id="+ QString::number(siteUniqueId));
//    query.exec();
//    if (query.next()) {
//        appUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Application (application_id, date_created, site_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, shipEntity->getApplicationId());
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, siteUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query.lastInsertId();
//        if(lastInserted.isValid())
//            appUniqueId = lastInserted.toInt();
//        else
//            appUniqueId = -1;
//    }
//    query.prepare("SELECT id FROM Entity WHERE entity_id=" + QString::number(shipEntity->getEntityId())+" AND application_id="+ QString::number(appUniqueId));
//    query.exec();
//    if (query.next()) {
//        entityUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Entity (entity_id, date_created, application_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, shipEntity->getEntityId());
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, appUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query.lastInsertId();
//        if(lastInserted.isValid())
//            entityUniqueId = lastInserted.toInt();
//        else
//            entityUniqueId = -1;
//    }

    query.prepare("INSERT INTO ShipMotionData (simulation_id, site_id, application_id, entity_id, current_msec_from_epoch, time_step, lat, lon, "
                  "simulation_time, os, cog, sog, hdg, rot, ukc, stw, rpm1, rpm2, pitch1, pitch2, eot1, eot2, thrust1, thrust2, cmdThrust1, cmdThrust2, "
                  "actRudder1, actRudder2, cmdRudder1, cmdRudder2, apStatus, apComCourse, exercise_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.bindValue(0, shipEntity->getSimulationId());
    query.bindValue(1, siteUniqueId);
    query.bindValue(2, shipEntity->getApplicationId());
    query.bindValue(3, shipEntity->getEntityId());
    query.bindValue(4, shipEntity->getTimeStamp());
    query.bindValue(5, timeStep);
    query.bindValue(6, shipEntity->getLat());
    query.bindValue(7, shipEntity->getLon());
    query.bindValue(8, shipEntity->getSimulationTime());
    query.bindValue(9, shipEntity->getOs());
    query.bindValue(10, shipEntity->getCog());
    query.bindValue(11, shipEntity->getSog());
    query.bindValue(12, shipEntity->getHdg());
    query.bindValue(13, shipEntity->getRot());
    query.bindValue(14, shipEntity->getUkc());
    query.bindValue(15, shipEntity->getStw());
    query.bindValue(16, shipEntity->getRpm1());
    query.bindValue(17, shipEntity->getRpm2());
    query.bindValue(18, shipEntity->getPitch1());
    query.bindValue(19, shipEntity->getPitch2());
    query.bindValue(20, shipEntity->getEot1());
    query.bindValue(21, shipEntity->getEot2());
    query.bindValue(22, shipEntity->getThrust1());
    query.bindValue(23, shipEntity->getThrust2());
    query.bindValue(24, shipEntity->getCmdThrust1());
    query.bindValue(25, shipEntity->getCmdThrust2());
    query.bindValue(26, shipEntity->getActRudder1());
    query.bindValue(27, shipEntity->getActRudder2());
    query.bindValue(28, shipEntity->getCmdRudder1());
    query.bindValue(29, shipEntity->getCmdRudder2());
    query.bindValue(30, shipEntity->getApStatus());
    query.bindValue(31, shipEntity->getApComCourse());
    query.bindValue(32, shipEntity->getExerciseId());

    query.exec();
    db.commit();

    QVariant lastInserted = query.lastInsertId();
    if(lastInserted.isValid())
        return true;
    else
        return false;
}

void DatabaseInterface::storeShipMotionData(QMap<QString, QString> &fixedDatumMap, int simulationId, QDateTime simulationStartTime)
{
    ShipMotionEntity* shipEntity = new ShipMotionEntity();

    shipEntity->setSimulationId(simulationId);
    shipEntity->setSiteId(fixedDatumMap["siteId"].toInt());
    shipEntity->setApplicationId(fixedDatumMap["applicationId"].toInt());
    shipEntity->setEntityId(fixedDatumMap["entityId"].toInt());
    shipEntity->setExerciseId(fixedDatumMap["exerciseId"].toInt());
    shipEntity->setTimeStamp(QDateTime::currentMSecsSinceEpoch());
    shipEntity->setLat(fixedDatumMap["lat"].toDouble());
    shipEntity->setLon(fixedDatumMap["lon"].toDouble());
    shipEntity->setSimulationTime(fixedDatumMap["simTime"].toLongLong());
    shipEntity->setOs(fixedDatumMap["os"].toInt());
    shipEntity->setCog(fixedDatumMap["cog"].toDouble());
    shipEntity->setSog(fixedDatumMap["sog"].toDouble());
    shipEntity->setHdg(fixedDatumMap["hdg"].toDouble());
    shipEntity->setRot(fixedDatumMap["rot"].toDouble());
    shipEntity->setUkc(fixedDatumMap["ukc"].toDouble());
    shipEntity->setStw(fixedDatumMap["stw"].toDouble());
    shipEntity->setRpm1(fixedDatumMap["rpm1"].toDouble());
    shipEntity->setRpm2(fixedDatumMap["rpm2"].toDouble());
    shipEntity->setPitch1(fixedDatumMap["pitch1"].toDouble());
    shipEntity->setPitch2(fixedDatumMap["pitch2"].toDouble());
    shipEntity->setEot1(fixedDatumMap["eot1"].toDouble());
    shipEntity->setEot2(fixedDatumMap["eot2"].toDouble());
    shipEntity->setThrust1(fixedDatumMap["thrust1"].toDouble());
    shipEntity->setThrust2(fixedDatumMap["thrust2"].toDouble());
    shipEntity->setCmdThrust1(fixedDatumMap["cmdThrust1"].toDouble());
    shipEntity->setCmdThrust2(fixedDatumMap["cmdThrust2"].toDouble());
    shipEntity->setActRudder1(fixedDatumMap["actRudder1"].toDouble());
    shipEntity->setActRudder2(fixedDatumMap["actRudder2"].toDouble());
    shipEntity->setCmdRudder1(fixedDatumMap["cmdRudder1"].toDouble());
    shipEntity->setCmdRudder2(fixedDatumMap["cmdRudder2"].toDouble());
    shipEntity->setApStatus(fixedDatumMap["apStatus"].toInt());
    shipEntity->setApComCourse(fixedDatumMap ["apComCourse"].toDouble());


    storeShipMotionData(shipEntity, simulationStartTime);
}

void DatabaseInterface::storeAISData(QString aisData, int simulationId, int siteId, int appId, int entityId,int exerciseId)
{
    QSqlQuery query(db), query2(db);
    int siteUniqueId, appUniqueId, entityUniqueId;

    query.prepare("SELECT id FROM Site WHERE site_id="+ QString::number(siteId));
    query.exec();
    if (query.next()) {
        siteUniqueId = query.value(0).toInt();
    } else {
        query2.prepare("INSERT INTO Site (site_id, date_created) VALUES (?, ?)");
        query2.bindValue(0, siteId);
        query2.bindValue(1, QDateTime::currentDateTime());

        query2.exec();
        db.commit();

        QVariant lastInserted = query2.lastInsertId();
        if(lastInserted.isValid())
            siteUniqueId = lastInserted.toInt();
        else
            siteUniqueId = -1;
    }
//    query.prepare("SELECT id FROM Application WHERE application_id=" + QString::number(appId)+" AND site_id="+ QString::number(siteUniqueId));
//    query.exec();
//    if (query.next()) {
//        appUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Application (application_id, date_created, site_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, appId);
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, siteUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query2.lastInsertId();
//        if(lastInserted.isValid())
//            appUniqueId = lastInserted.toInt();
//        else
//            appUniqueId = -1;
//    }
//    query.prepare("SELECT id FROM Entity WHERE entity_id=" + QString::number(entityId)+" AND application_id="+ QString::number(appUniqueId));
//    query.exec();
//    if (query.next()) {
//        entityUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Entity (entity_id, date_created, application_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, entityId);
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, appUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query2.lastInsertId();
//        if(lastInserted.isValid())
//            entityUniqueId = lastInserted.toInt();
//        else
//            entityUniqueId = -1;
//    }

    query.prepare("INSERT INTO AISData (simulation_id, site_id, application_id, entity_id, ais_data,exercise_id,timestamp_msec_epoch) VALUES (?, ?, ?, ?, ?,?,?)");
    query.bindValue(0, simulationId);
    query.bindValue(1, siteUniqueId);
    query.bindValue(2, appId);
    query.bindValue(3, entityId);
    query.bindValue(4, aisData);
    query.bindValue(5, exerciseId);
    query.bindValue(6, QDateTime::currentMSecsSinceEpoch());

    query.exec();
    db.commit();
}

void DatabaseInterface::storeNavLightsAndShapes(int simulationId, int siteId, int appId, int entityId,int exerciseId, int light, int shape)
{
//    QSqlQuery query(db), query2(db);
//    int siteUniqueId, appUniqueId, entityUniqueId;
//    bool update = false;

//    for(int i=0; i<navLightsAndShapes.size(); i++) {
//        if(navLightsAndShapes.at(i)->getSimulationId()==simulationId && navLightsAndShapes.at(i)->getSiteId()==siteId &&
//                navLightsAndShapes.at(i)->getAppId()==appId && navLightsAndShapes.at(i)->getEntityId()==entityId ) {
//            if(navLightsAndShapes.at(i)->getLights()==light && navLightsAndShapes.at(i)->getShapes()==shape)
//                return;
//            else {
//                update = true;
//                navLightsAndShapes.at(i)->setLights(light);
//                navLightsAndShapes.at(i)->setShapes(shape);
//                break;
//            }
//        }
//    }


//    query.prepare("SELECT id FROM Site WHERE site_id="+ QString::number(siteId));
//    query.exec();
//    if (query.next()) {
//        siteUniqueId = query.value(0).toInt();
//    } else {
//        return;
//    }
//    query.prepare("SELECT id FROM Application WHERE application_id=" + QString::number(appId)+" AND site_id="+ QString::number(siteUniqueId));
//    query.exec();
//    if (query.next()) {
//        appUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Application (application_id, date_created, site_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, appId);
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, siteUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query.lastInsertId();
//        if(lastInserted.isValid())
//            appUniqueId = lastInserted.toInt();
//        else
//            appUniqueId = -1;
//    }
//    query.prepare("SELECT id FROM Entity WHERE entity_id=" + QString::number(entityId)+" AND application_id="+ QString::number(appUniqueId));
//    query.exec();
//    if (query.next()) {
//        entityUniqueId = query.value(0).toInt();
//    } else {
//        query2.prepare("INSERT INTO Entity (entity_id, date_created, application_id) VALUES (?, ?, ?)");
//        query2.bindValue(0, entityId);
//        query2.bindValue(1, QDateTime::currentDateTime());
//        query2.bindValue(2, appUniqueId);

//        query2.exec();
//        db.commit();

//        QVariant lastInserted = query.lastInsertId();
//        if(lastInserted.isValid())
//            entityUniqueId = lastInserted.toInt();
//        else
//            entityUniqueId = -1;
//    }

    //&& appUniqueId!=-1 && entityUniqueId!=-1


//        if(update) {
//            query.prepare("UPDATE NavLightsAndShapes SET lights=" + QString::number(light)+ ", shapes=" + QString::number(shape) +
//                          " WHERE simulation_id="+QString::number(simulationId) + " AND site_id= "+QString::number(siteUniqueId)+
//                          " AND application_id="+QString::number(appId) + " AND exercise_id="+QString::number(exerciseId) + " AND entity_id="+QString::number(entityId));

//            query.exec();
//        } else {
//            query.prepare("INSERT INTO NavLightsAndShapes (simulation_id, site_id, application_id, entity_id, lights, shapes,exercise_id,timestamp_msec_epoch) VALUES (?, ?, ?, ?, ?, ?,?,?)");
//            query.bindValue(0, simulationId);
//            query.bindValue(1, siteUniqueId);
//            query.bindValue(2, appId);
//            query.bindValue(3, entityId);
//            query.bindValue(4, light);
//            query.bindValue(5, shape);
//            query.bindValue(6, exerciseId);
//            query.bindValue(7, QDateTime::currentMSecsSinceEpoch());

//            query.exec();

//            NavLightsAndShapes *newInstance = new NavLightsAndShapes();
//            newInstance->setSimulationId(simulationId);  newInstance->setSiteId(siteId);  newInstance->setAppId(appId);  newInstance->setEntityId(entityId);
//            newInstance->setLights(light);  newInstance->setShapes(shape);
//            navLightsAndShapes.append(newInstance);
//        }


}


QList<int> DatabaseInterface::getSimulationIds()
{
    QList<int> result;
    QSqlQuery query(db);
    query.prepare("SELECT id FROM SimulationInstance");
    query.exec();
    while (query.next()) {
        result << query.value(0).toInt();
    }
    return result;
}

QList<int> DatabaseInterface::getSiteIds()
{
    QList<int> result;
    QSqlQuery query(db);
    query.prepare("SELECT site_id FROM Site");
    query.exec();
    while (query.next()) {
        result << query.value(0).toInt();
    }
    return result;
}

QMap<int, SimulationInformation> DatabaseInterface::getSimulationIdWithData()
{
    QMap<int,SimulationInformation> result;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM SimulationInstance");
    query.exec();


    while (query.next()) {
        SimulationInformation _simInfo;

        _simInfo.setStartDateTime(query.value(1).toDateTime());
        _simInfo.setEndDateTime(query.value(2).toDateTime());
        _simInfo.setSuccess(query.value(3).toInt());
        _simInfo.setName(query.value(4).toString());
        _simInfo.setDetails(query.value(5).toString());
        _simInfo.setSimulationID(query.value(0).toInt());
        result[_simInfo.getSimulationID()] = _simInfo;
    }
    return result;
}


QList<ShipMotionEntity *> DatabaseInterface::getShipMotionData(int simId, int appId, int siteId, int entityId)
{
    QList<ShipMotionEntity*> result;
    QSqlQuery query(db);
    int siteUniqueId, appUniqueId, entityUniqueId;
    QString mainQueryStr = "SELECT site_id, application_id, entity_id, current_msec_from_epoch, time_step, lat, lon, "
                           "simulation_time, os, cog, sog, hdg, rot, ukc, stw, rpm1, rpm2, pitch1, pitch2, eot1, eot2, thrust1, thrust2, cmdThrust1, cmdThrust2, "
                           "actRudder1, actRudder2, cmdRudder1, cmdRudder2, apStatus, apComCourse FROM ShipMotionData WHERE simulation_id="+QString::number(simId);

    if(siteId!=-1) {
        query.prepare("SELECT id FROM Site WHERE site_id="+QString::number(siteId));
        query.exec();
        if (query.next()) {
            siteUniqueId = query.value(0).toInt();
            mainQueryStr += " AND site_id="+QString::number(siteUniqueId);
        } else
            siteUniqueId = -1;

        if(appId!=-1) {
            query.prepare("SELECT id FROM Application WHERE site_id="+QString::number(siteUniqueId)+" AND application_id="+QString::number(appId));
            query.exec();
            if (query.next()) {
                appUniqueId = query.value(0).toInt();
                mainQueryStr += " AND application_id="+QString::number(appUniqueId);
            } else
                appUniqueId = -1;

            if(entityId!=-1) {
                query.prepare("SELECT id FROM Entity WHERE site_id="+QString::number(siteUniqueId)+" AND entity_id="+QString::number(entityId));
                query.exec();
                if (query.next()) {
                    entityUniqueId = query.value(0).toInt();
                    mainQueryStr += " AND entity_id="+QString::number(entityUniqueId);
                } else
                    entityUniqueId = -1;
            }
        }
    }
    mainQueryStr += " ORDER BY time_step";


    query.prepare(mainQueryStr);
    query.exec();
    qDebug() << query.executedQuery();

    while (query.next()) {
        ShipMotionEntity* shipEntity = new ShipMotionEntity();

        shipEntity->setSiteId(query.value(0).toInt());
        shipEntity->setApplicationId(query.value(1).toInt());
        shipEntity->setEntityId(query.value(2).toInt());
        shipEntity->setTimeStamp(query.value(3).toLongLong());
        shipEntity->setTimeStep(query.value(4).toInt());
        shipEntity->setLat(query.value(5).toDouble());
        shipEntity->setLon(query.value(6).toDouble());
        shipEntity->setSimulationTime(query.value(7).toLongLong());
        shipEntity->setOs(query.value(8).toInt());
        shipEntity->setCog(query.value(9).toDouble());
        shipEntity->setSog(query.value(10).toDouble());
        shipEntity->setHdg(query.value(11).toDouble());
        shipEntity->setRot(query.value(12).toDouble());
        shipEntity->setUkc(query.value(13).toDouble());
        shipEntity->setStw(query.value(14).toDouble());
        shipEntity->setRpm1(query.value(15).toDouble());
        shipEntity->setRpm2(query.value(16).toDouble());
        shipEntity->setPitch1(query.value(17).toDouble());
        shipEntity->setPitch2(query.value(18).toDouble());
        shipEntity->setEot1(query.value(19).toDouble());
        shipEntity->setEot2(query.value(20).toDouble());
        shipEntity->setThrust1(query.value(21).toDouble());
        shipEntity->setThrust2(query.value(22).toDouble());
        shipEntity->setCmdThrust1(query.value(23).toDouble());
        shipEntity->setCmdThrust2(query.value(24).toDouble());
        shipEntity->setActRudder1(query.value(25).toDouble());
        shipEntity->setActRudder2(query.value(26).toDouble());
        shipEntity->setCmdRudder1(query.value(27).toDouble());
        shipEntity->setCmdRudder2(query.value(28).toDouble());
        shipEntity->setApStatus(query.value(29).toInt());
        shipEntity->setApComCourse(query.value(30).toDouble());

        result.append(shipEntity);
    }
    return result;

}


void DatabaseInterface::closeDB()
{
    db.close() ;
    qDebug()<<"Db Closed Successfully!!";
}

bool DatabaseInterface::openDB()
{


    if(opened){
        db.close();
        opened =false;
    }
  bool success = false;

    if(!opened) {


        db = QSqlDatabase::addDatabase(DB_DRIVER, DB_ID);
        qDebug() << DB_CONNECTION_SETTINGS;
        db.setDatabaseName(DB_CONNECTION_SETTINGS);

        // open connection
        success = db.open();
        if(success){
            qDebug("DB connection succeeded!");
            opened = true;
        }else{
            qDebug("DB connection failed!");
            opened = false;
        }
    }

    return success;
}


void DatabaseInterface::selectDataBase(int db){
    switch ((DB_TYPE)db) {
    case CML_DATA_SV:
        host = "CML-DATA-SV";
        port = "1433";
        database ="EMSN_DATA";
        DB_DRIVER = "QODBC3";
        DB_ID = "EMSN_DATA_SV";

        DB_CONNECTION_SETTINGS = "DRIVER={SQL Server};Server=" +host+";Port="+port+";Database="+database+";Integrated Security=True;";

        break;
    case CML_RS_SIM:

        host = "CML-RS-SIM";
        port = "1433";
        database ="EMSN_DATA";
        DB_DRIVER = "QODBC3";
        DB_ID = "EMSN_RS_SIM";
        DB_CONNECTION_SETTINGS = "DRIVER={SQL Server};Server=" +host+";Port="+port+";Database="+database+";Integrated Security=True;";
        break;
    case CML_Local:

        host = "CML-DATA-SV";
        port = "1433";
        database ="EMSN_DATA";
        DB_DRIVER = "QODBC3";
        DB_ID = "EMSN_Local";

        DB_CONNECTION_SETTINGS = "DRIVER={Microsoft Access Driver (*.mdb)};DSN='';DBQ=D:\\EMSN_data.mdb";
        break;


    }


#ifdef DATA_TRACKER
    if(openDB()==false) {
        qDebug() << "ERROR: Could not open database.";
        //exit(-1);
    }

#endif
}

