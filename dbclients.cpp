#include "dbclients.h"

DbWriter::DbWriter(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(dummyJsonForTesting(QByteArray)),this, SLOT(receiveJsonFromServer(QByteArray)));
    connect(this, SIGNAL(sendDecodedSensorData(dbData)), this, SLOT(storeDataIntoServer(dbData)));
}

bool DbWriter::openDB(QString url)
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(url);
    //    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + QString("T:\\Transfer\\STNS HiWi-Projektordner\\Ashraful\\WEB-SPI-Uncommited\\web-sbi\\member.mdb"));

    if(db.open())
    {
        opened =true;
    }
    else
    {
        opened =false;
        qDebug()<<"Error: "<<db.lastError().text();
    }
    return opened;
}

void DbWriter::closeDB()
{
    db.close() ;
    qDebug()<<"Db Closed Successfully!!";
}

void DbWriter::importDataFromServer()
{
    QStringList _data;
    if(opened){
        QSqlQuery query(db);
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
        qDebug()<<data2;
        _data.clear();
    }
}
}

void DbWriter::receiveJsonFromServer(QByteArray array)
{        
    readListOfDevices();
    dbData dataFromServer;

    QJsonDocument docs= QJsonDocument::fromJson(array);
    dataFromServer.node_id = docs.object().value("sourceId").toInt();

    QJsonObject data = docs.object().value("data").toObject();
    dataFromServer.timestamp = data.value("time").toDouble();
    dataFromServer.date_time=QDateTime::currentDateTime().toString("dd MM yyyy hh:mm:ss");
    dataFromServer.lat = data.value("lat").toDouble();
    dataFromServer.lon = data.value("lon").toDouble();

    QJsonArray sensor_data = data.value("sensorData").toArray();
    double sensor_[sensor_data.size()];    

    for(unsigned int i=0;i<sensor_data.size();i++)
    {
        int id = sensor_data[i].toObject().value("id").toInt();
        double value = sensor_data[i].toObject().value("value").toDouble();
        dataFromServer.sensors.insert(id,value);
        sensor_[i] = sensor_data[i].toObject().value("value").toDouble();
    }   

    dataFromServer.course = data.value("cog").toDouble();
    dataFromServer.device_id= docs.object().value("sourceId").toDouble();

    emit sendDecodedSensorData(dataFromServer);    

    double sourceId=docs.object().value("sourceId").toDouble();
    QString sourceName=docs.object().value("sourceName").toString();

    if(!listOfDevices.contains(sourceId))
    {
        listOfDevices.insert(sourceId,sourceName);
        writeIntoDevicesList(sourceId,sourceName);
}
    //Write sensor's id and name into Config file//

    sensorList.clear();
    SensorData _sensorData;
    _sensorData.device_id =sourceId;
    _sensorData.device_name =sourceName;  

    for(int i=0;i<sensor_data.size();i++)
    {        
        _sensorData.id=sensor_data[i].toObject().value("id").toInt();
        _sensorData.sensor_name=sensor_data[i].toObject().value("name").toString();
        sensorList.append(_sensorData);        
    }

    QSettings sensorDetails(qApp->applicationDirPath()+QString("/config_sensor.ini"),QSettings::IniFormat);
    sensorDetails.beginGroup("SensorDetails");
    int numOfSensors = sensorDetails.beginReadArray("Sensors");
    sensorDetails.endArray();
    sensorDetails.endGroup();
    qDebug()<<numOfSensors;


    sensorDetails.beginGroup("SensorDetails");
    sensorDetails.beginWriteArray("Sensors", numOfSensors+sensorList.size());
    for (int i = numOfSensors; i<(numOfSensors+sensorList.size()); i++) {
        sensorDetails.setArrayIndex(i);
        _sensorData = sensorList.at(i-numOfSensors);
        sensorDetails.setValue("Device_Id",_sensorData.device_id);
        sensorDetails.setValue("Device_Name", _sensorData.device_name);
        sensorDetails.setValue("Sensor_Id", _sensorData.id);
        sensorDetails.setValue("Sensor_Name", _sensorData.sensor_name);
        qDebug()<<"Going on";
    }
    sensorDetails.endArray();
    sensorDetails.endGroup();
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

void DbWriter::createJsonForTesting()
{
    for(int i=0;i<1000;i++)
    {
        double lat=55.340021 + i;
        double lon=46.120043 - i;

        QJsonArray sensor_data;

        for(int i = 0; i<4; i++)
        {
            QJsonObject data_1= QJsonObject{
            {"id", i+1},
            {"value", 36.4+i},
            {"unit", "deg"},
            {"name", "data_"+QString::number(i)},
        };

            sensor_data.push_back(data_1);
        }

        QJsonObject data{
            {"lat", lat},
            {"lon", lon},
            {"time", 52436125},
            {"cog", 20},
            {"sensorData", sensor_data},
        };

        QJsonObject object{
            {"sourceId", 1},
            {"sourceName", "TestShip1"},
            {"dataId", 1},
            {"data", data},
        };

        QJsonDocument document = QJsonDocument(object);

        emit dummyJsonForTesting(document.toJson());
        qDebug()<<"QJsonObject is created and emitted...";
    }
}

void DbWriter::writeIntoDevicesList(double id, QString name)
{
    if(opened){
        QSqlQuery query(db);

        query.prepare("INSERT INTO device_id (sourceId, sourceName) VALUES (?, ?)");
        query.bindValue(0,id);
        query.bindValue(1,name);

        query.exec();
        db.commit();
        qDebug()<<"New Device enlisted in Database.";
    }
}

void DbWriter::storeDataIntoServer(dbData object)
{
    QMapIterator<int,double> sensors(object.sensors);

    if(opened){
        QSqlQuery query(db);

        query.prepare("INSERT INTO sensor_data (node_id, [timestamp], date_time, lat, lon, sensor_1, sensor_2, sensor_3, sensor_4, sensor_5, sensor_6, course, device_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.bindValue(0, object.node_id);
        query.bindValue(1, object.timestamp);
        query.bindValue(2, object.date_time);
        query.bindValue(3, object.lat);
        query.bindValue(4, object.lon);
        while(sensors.hasNext()){
            sensors.next();
            int id= sensors.key()+4;
            double value = sensors.value();
            qDebug()<<sensors.key()<<value;
            query.bindValue(id,value);
        }
        query.bindValue(11, object.course);
        query.bindValue(12, object.device_id);

        query.exec();
        db.commit();
        qDebug()<<"Struct data is received and inserted to the database...";
    }
}

void DbWriter::readListOfDevices()
{
    if(opened){
        QSqlQuery query(db);
        query.prepare("SELECT [sourceId], [sourceName] FROM device_id");
        query.exec();
        while (query.next())
        {
            qDebug()<<"Start reading devices";
            listOfDevices.insert(query.value(0).toDouble(),query.value(1).toString());
            qDebug()<<query.value(0).toDouble()<<query.value(1).toString();

        };


    }

}

