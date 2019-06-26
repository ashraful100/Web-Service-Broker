#include "dbwriter.h"

DbWriter::DbWriter(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(dummyOwnShipJsonForTesting(QByteArray)),this, SLOT(receivedOwnShipFromServer(QByteArray)));
    connect(this, SIGNAL(dummyTrafficShipJsonForTesting(QByteArray)),this, SLOT(receivedTrafficShipFromServer(QByteArray)));
    connect(this, SIGNAL(sendDecodedSensorData(dbData)), this, SLOT(storeDataIntoDatabase(dbData)));

    /* Read sensor details from config_sensor file */
    QSettings sensorDetails(qApp->applicationDirPath()+QString("/config_sensor.ini"),QSettings::IniFormat);
    sensorDetails.beginGroup("SensorDetails");
    size = sensorDetails.beginReadArray("Sensors");
    SensorData _sensorData;
    if(size>0)
    {
        for(int k=0;k<size;k++)
        {
            sensorDetails.setArrayIndex(k);
            _sensorData.device_id=sensorDetails.value("Device_Id").toInt();
            _sensorData.sensor_id=sensorDetails.value("Sensor_Id").toInt();
            _sensorData.sensor_name=sensorDetails.value("Sensor_Name").toString();
            _sensorData.sensor_unit=sensorDetails.value("Sensor_Unit").toString();

            _sensorConfig.insert(k+1,_sensorData);
            if(!deviceId.contains(_sensorData.device_id))
            {
                deviceId.append(_sensorData.device_id);
            }
        }
        sensorDetails.endArray();
        sensorDetails.endGroup();
    }
}

bool DbWriter::openDB(int id, QString osUrl, QString tsUrl)
{
    ownShip_db = QSqlDatabase::addDatabase("QODBC", "EMSN_OWN_SHIP");
    ownShip_db.setDatabaseName(osUrl);

    trafficShip_db = QSqlDatabase::addDatabase("QODBC", "EMSN_TRAFFIC_SHIP");
    trafficShip_db.setDatabaseName(tsUrl);

    if(ownShip_db.open() && trafficShip_db.open())
    {
        qDebug()<<"sensor_data_local is connected";
        qDebug()<<"traffic_ship_data is connected";
        opened =true;
    }
    else
    {
        opened =false;
        qDebug()<<"Error 1: "<<ownShip_db.lastError().text()<<"Error 2: "<<trafficShip_db.lastError().text();;
    }
    os_Url=osUrl;
    ts_Url=tsUrl;

    db_ID= id;
    return opened;
}

void DbWriter::closeDB()
{
    ownShip_db.close() ;
    trafficShip_db.close();
}

void DbWriter::importDataFromServer()
{
    QStringList _data;
    if(opened){
        QSqlQuery query(ownShip_db);
        query.prepare("SELECT TOP 1000 [id] ,[node_id] ,[timestamp] ,[date_time], [lat], [lon], [sensor_1], [sensor_2], [sensor_3], [sensor_4], [course], [device_id] FROM sensor_data");
        query.exec();
        while (query.next())
        {
            for(int i=0; i<12;i++)
            {
                _data.push_back(query.value(i).toString());
            }

            QJsonArray sensor_data;

            for(int i = 0; i<4; i++)
            {
                QJsonObject obj= QJsonObject{
                {"value", _data[i+6]},
            };
            sensor_data.push_back(obj);
        };

        QJsonObject data{
            {"lat", _data[4]},
            {"lon", _data[5]},
            {"time", _data[2]},
            {"sensorData", sensor_data},
        };

        QJsonObject data2{
            {"sourceId", _data[0]},
            {"dataId",_data[1]},
            {"data", data},
        };

        QJsonDocument document = QJsonDocument(data2);

        emit sendJsonToServer(document.toJson());
        _data.clear();
    }
}
}

void DbWriter::receivedOwnShipFromServer(QByteArray array)
{            
    bool validity1, validity2;
    readListOfDevices();
    dbData dataFromServer;

    QJsonDocument docs= QJsonDocument::fromJson(array);
    QJsonObject data = docs.object().value("data").toObject();
    int sourceId=docs.object().value("sourceId").toString().toInt();
    QString sourceName=docs.object().value("sourceName").toString();
    dataFromServer.node_id = docs.object().value("sourceId").toString().toInt();

    QJsonArray gpsData= data.value("gpsData").toArray();
    for(int i=0; i<gpsData.size();i++)
    {
        QJsonObject gpsDataObj = gpsData[i].toObject();
        int id=gpsDataObj.value("id").toString().toInt();
        qDebug()<<"Id is"<<id;
        double latAis,lonAis,latReach,lonReach;
        if(id==1)
        {
            latAis=gpsDataObj.value("latitude").toString().toDouble();
            lonAis=gpsDataObj.value("longitude").toString().toDouble();
            qDebug()<<id<<latAis<<lonAis;
            QGeoCoordinate pointAis= QGeoCoordinate(latAis,lonAis);
            validity1=pointAis.isValid();
            if(validity1)
            {
                dataFromServer.lat_ais=latAis;
                dataFromServer.lon_ais=lonAis;
            }
        }
        if(id==2)
        {
            latReach=gpsDataObj.value("latitude").toString().toDouble();
            lonReach=gpsDataObj.value("longitude").toString().toDouble();
            qDebug()<<id<<latReach<<lonReach;
            QGeoCoordinate pointReach= QGeoCoordinate(latReach,lonReach);
            validity2=pointReach.isValid();
            if(validity2)
            {
                dataFromServer.lat_reach=latReach;
                dataFromServer.lon_reach=lonReach;
            }
        }
    }

    dataFromServer.date_time=QDateTime::currentDateTime();//.toString("dd MM yyyy hh:mm:ss");
    QJsonArray sensor_data = data.value("sensorData").toArray();
    QMap<int,double> sensors;
    for(int i=0;i<sensor_data.size();i++)
    {
        QJsonObject sensor_data_obj =sensor_data[i].toObject();
        int id = sensor_data_obj.value("id").toString().toInt();
        double value = sensor_data_obj.value("value").toString().toDouble();
        sensors[id] = value;
    }
    dataFromServer.sensors = sensors;
    dataFromServer.course = data.value("cog").toString().toDouble();
    dataFromServer.device_id= docs.object().value("sourceId").toString().toInt();
    dataFromServer.timestamp = docs.object().value("time").toString().toLongLong();
    qDebug()<<dataFromServer.timestamp;//<<docs.object().value("time").toString().toLongLong();

    if(validity1 || validity2)
    {
        emit sendDecodedSensorData(dataFromServer);
    }

    if(!listOfDevices.contains(sourceId))
    {
        listOfDevices.insert(sourceId,sourceName);
        writeIntoDevicesDatabase(sourceId,sourceName);

        sensorDb.clear();
        SensorData _sensorData;
        _sensorData.device_id =sourceId;

        for(int i=0;i<sensor_data.size();i++)
        {
            _sensorData.sensor_id=sensor_data[i].toObject().value("id").toString().toInt();
            _sensorData.sensor_name=sensor_data[i].toObject().value("name").toString();
            _sensorData.sensor_unit=sensor_data[i].toObject().value("unit").toString();
            sensorDb.append(_sensorData);
        }

        writeIntoSensorDatabase(sensorDb);
    }

    /* Write sensor details to config_sensor file */
    if(!deviceId.contains(sourceId))
    {
        SensorData _sensorData;
        _sensorData.device_id =sourceId;
        for(int i=0;i<sensor_data.size();i++)
        {
            _sensorData.sensor_id=sensor_data[i].toObject().value("id").toString().toInt();
            _sensorData.sensor_name=sensor_data[i].toObject().value("name").toString();
            _sensorData.sensor_unit=sensor_data[i].toObject().value("unit").toString();
            _sensorConfig.insert(size+i+1,_sensorData);
        }
        deviceId.append(sourceId);
    }
}

void DbWriter::receivedTrafficShipFromServer(QByteArray array)
{    
//    qDebug()<<"Here it is"<<os_Url<<ts_Url;
    QJsonDocument docs= QJsonDocument::fromJson(array);
    QJsonArray trafficShipArray = docs.object().value("trafficShipData").toArray();
    for(int i=0; i<trafficShipArray.size();i++)
    {
        trafficShipData data;
        QJsonObject trafficShip = trafficShipArray[i].toObject();
        data.heading=trafficShip.value("heading").toDouble();
        data.latitude=trafficShip.value("latitude").toDouble();
        data.longitude=trafficShip.value("longitude").toDouble();
        data.mmsi=trafficShip.value("mmsi").toString();
        qDebug()<<"MMSI value is"<<data.mmsi;
        data.time_Stamp=trafficShip.value("time_Stamp").toString();
        storeTrafficDataIntoDatabase(data);
    }
}

void DbWriter::selectDB()
{
    //        switch ((DB_TYPE)db) {
    //        case CML_DATA_SV:
    //            host = "CML-DATA-SV";
    //            port = "1433";
    //            database ="EMSN_DATA";
    //            DB_DRIVER = "QODBC3";
    //            DB_ID = "EMSN_DATA_SV";

    //            DB_CONNECTION_SETTINGS = "DRIVER={SQL Server};Server=" +host+";Port="+port+";Database="+database+";Integrated Security=True;";

    //            break;
    //        case CML_RS_SIM:
    //            host = "CML-RS-SIM";
    //            port = "1433";
    //            database ="EMSN_DATA";
    //            DB_DRIVER = "QODBC3";
    //            DB_ID = "EMSN_RS_SIM";
    //            DB_CONNECTION_SETTINGS = "DRIVER={SQL Server};Server=" +host+";Port="+port+";Database="+database+";Integrated Security=True;";
    //            break;
    //        case CML_Local:
    //            host = "CML-DATA-SV";
    //            port = "1433";
    //            database ="EMSN_DATA";
    //            DB_DRIVER = "QODBC3";
    //            DB_ID = "EMSN_Local";

    //            DB_CONNECTION_SETTINGS = "DRIVER={Microsoft Access Driver (*.mdb)};DSN='';DBQ=D:\\EMSN_data.mdb";
    //            break;
    //        }
}

void DbWriter::createOwnShipDataJsonForTesting()
{
    qDebug()<<os_Url<<ts_Url;
    int x=3;
    QDateTime currentTime;
    qint64 timeStamp= currentTime.currentMSecsSinceEpoch();
    qDebug()<<timeStamp;
    for(int i=0;i<100;i++)
    {
        double lat = -45.000000123 + (i*0.01);
        double lon = -90.000000126 + (i*0.02);

        QJsonArray sensor_data;

        for(int i = 0; i<8; i++)
        {
            double k=i+30.8236+ (x*0.1);
            QJsonObject data_1 = QJsonObject{
            {"id", QString::number(i+1)},
            {"value", QString::number(k)},
            {"unit", "atm"},
            {"name", "sensor_"+QString::number(i)},
        };
            sensor_data.push_back(data_1);
        }
        x++;

        QJsonObject aisData{
            {"id",1},
            {"latitude",QString::number(lat)},
            {"longitude",QString::number(lon)},
            {"source","ais"}
        };
        QJsonObject reachData{
            {"id",2},
            {"latitude",QString::number(lat)},
            {"longitude",QString::number(lon)},
            {"source","reach"}
        };
        QJsonArray gps_data_array;

        gps_data_array.append(aisData);
        //        gps_data_array.append(reachData);

        QJsonObject data{
            {"gpsData", gps_data_array},
            {"cog", 20},
            {"sensorData", sensor_data},
        };

        QJsonObject object{
            {"sourceId", "210"},
            {"sourceName", "Bremen Ship"},
            {"dataId", "1"},
            {"time", QString::number(timeStamp+i*1000)},
            {"data", data},
        };
        qDebug()<<object;

        QJsonDocument document = QJsonDocument(object);

        emit dummyOwnShipJsonForTesting(document.toJson());
        //        qDebug()<<"QJsonObject is created and emitted..."<<document;
    }
}

void DbWriter::createTrafficShipDataJsonForTesting()
{
    QDateTime currentTime;
    qint64 timestamp= currentTime.currentMSecsSinceEpoch();
    QJsonArray trafficShipArray;

    for(int i =0;i<1000;i++)
    {
        QJsonObject obj= QJsonObject{
            {"heading",32+(i*0.001)},
            {"latitude",double(35+i*0.02)},
            {"longitude",double(85-i*0.02)},
            {"mmsi", QString::number(10462354564)},
            {"time_stamp", QString::number(timestamp)},
        };
        trafficShipArray.push_back(obj);
    }

    QJsonObject obj2{
        {"trafficShipData", trafficShipArray},
    };
    QJsonDocument docs = QJsonDocument(obj2);

    emit dummyTrafficShipJsonForTesting(docs.toJson());
}

void DbWriter::writeIntoDevicesDatabase(int id, QString name)
{   
    if(opened){
        QSqlQuery query(ownShip_db);

        query.prepare("INSERT INTO device_id (sourceId, sourceName) VALUES (?, ?)");
        query.bindValue(0,id);
        query.bindValue(1,name);
        query.exec();
        ownShip_db.commit();
    }
}

void DbWriter::writeIntoSensorDatabase(QList<SensorData> sensors)
{    
    if(opened){
        for(int i=0;i<sensors.size();i++)
        {
            QSqlQuery query(ownShip_db);
            query.prepare("INSERT INTO sensor_list (device_id, sensor_id, sensor_name, sensor_unit) VALUES (?, ?, ?, ?)");
            query.bindValue(0,sensors.at(i).device_id);
            query.bindValue(1,sensors.at(i).sensor_id);
            query.bindValue(2,sensors.at(i).sensor_name);
            query.bindValue(3,sensors.at(i).sensor_unit);
            query.exec();
            ownShip_db.commit();
        }
    }
}

DbWriter::~DbWriter()
{
    /* Write sensor details to config_sensor file */
    QSettings sensorDetails(qApp->applicationDirPath()+QString("/config_sensor.ini"),QSettings::IniFormat);
    sensorDetails.beginGroup("SensorDetails");
    sensorDetails.beginWriteArray("Sensors", _sensorConfig.size());
    for (int i =0 ; i<_sensorConfig.size(); i++) {
        sensorDetails.setArrayIndex(i);
        sensorDetails.setValue("Device_Id",_sensorConfig[i+1].device_id);
        sensorDetails.setValue("Sensor_Id",_sensorConfig[i+1].sensor_id);
        sensorDetails.setValue("Sensor_Name",_sensorConfig[i+1].sensor_name);
        sensorDetails.setValue("Sensor_Unit",_sensorConfig[i+1].sensor_unit);
    }
    sensorDetails.endArray();
    sensorDetails.endGroup();
}

void DbWriter::storeDataIntoDatabase(dbData object)
{    
    if(db_ID==0)
    {
        tableName= "sensor_data";
    }
    else if(db_ID==1)
    {
        tableName= "[SENSOR_TERMINAL].[dbo].[sensor_data]";
    }

    if(opened){
        QSqlQuery query(ownShip_db);
        QMap<int,double> sensors = object.sensors;
        query.prepare("INSERT INTO "+tableName+" (node_id, time_stamp, date_time, lat, lon, lat_reach, lon_reach, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, sensor_7, sensor_8, course, device_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, object.node_id);

        if(db_ID==0){
            query.bindValue(1, QString::number(object.timestamp));
        }

        else{
            query.bindValue(1, object.timestamp);
        }

        query.bindValue(2, object.date_time);
        query.bindValue(3, object.lat_ais);
        query.bindValue(4, object.lon_ais);
        query.bindValue(5, object.lat_reach);
        query.bindValue(6, object.lon_reach);
        query.bindValue(7,         sensors.value(1));
        query.bindValue(8,         sensors.value(2));
        query.bindValue(9,         sensors.value(3));
        query.bindValue(10,         sensors.value(4));
        query.bindValue(11,         sensors.value(5));
        query.bindValue(12,        sensors.value(6));
        query.bindValue(13,        sensors.value(7));
        query.bindValue(14,        sensors.value(8));
        query.bindValue(15, object.course);
        query.bindValue(16, object.device_id);

        if(query.exec()){
        } else{
            qDebug() << query.lastError();
        }
    }
}

void DbWriter::storeTrafficDataIntoDatabase(trafficShipData data)
{    
    qDebug()<<"I received data"<<ts_Url;

    if(opened){
        QSqlQuery query(trafficShip_db);
        query.prepare("INSERT INTO traffic_ais_data (heading, latitude, longitude, mmsi, time_stamp) VALUES (?, ?, ?, ?, ?)");
        query.bindValue(0,data.heading);
        query.bindValue(1,data.latitude);
        query.bindValue(2,data.longitude);
        query.bindValue(3,data.mmsi);
        query.bindValue(4,data.time_Stamp);
        query.exec();
        trafficShip_db.commit();
    }
}

void DbWriter::readListOfDevices()
{
    if(db_ID==0)
    {
        tableName= "device_id";
    }
    else if(db_ID==1)
    {
        tableName= "[SENSOR_TERMINAL].[dbo].[device_id]";
    }

    if(opened){
        QSqlQuery query(ownShip_db);
        query.prepare("SELECT [sourceId], [sourceName] FROM "+tableName);
        query.exec();
        while (query.next())
        {
            listOfDevices.insert(query.value(0).toDouble(),query.value(1).toString());
        };
    }
}
