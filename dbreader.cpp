#include "dbreader.h"

DbReader::DbReader(QString osUrl, QString tsUrl, QString userId, QObject *parent) : QObject(parent)
{
    //    db = QSqlDatabase::addDatabase("QODBC");
    //    db.setDatabaseName(dbUrl);    
    opened=openDB(osUrl,tsUrl);
    _communicator=new Communicator("DB");
    _communicator->setTransmitterKey("K_"+userId);

    connect(this, SIGNAL(sendSensorDataToWebInterface(QByteArray)),_communicator,SIGNAL(sendTpMsg(QByteArray)));
    qDebug()<<"K_"+userId;
    qDebug()<<"communicator key = " << _communicator->transmitterKey;
    qDebug()<<"Here 2"<<osUrl<<tsUrl;
}

bool DbReader::openDB(QString osUrl,QString tsUrl)
{
    qDebug()<<"DbReader::openDB() receives signal";
    os_db = QSqlDatabase::addDatabase("QODBC");
    os_db.setDatabaseName(osUrl);

    ts_db = QSqlDatabase::addDatabase("QODBC");
    ts_db.setDatabaseName(tsUrl);

    if(os_db.open() && ts_db.open())
    {
        opened =true;
        qDebug()<<"DB is opened";
    }
    else
    {
        opened =false;
        qDebug()<<"OwnShip Error: "<<os_db.lastError().text()<<"TrafficShip Error: "<<ts_db.lastError().text();
        qDebug()<<"DB is not opened"<<osUrl<<tsUrl;
    }
    return opened;
}

void DbReader::closeDB()
{
    os_db.close();
    qDebug()<<"Db Closed Successfully!!";
}

void DbReader::sendDeviceList()
{
    //    opened=openDB()
    qDebug()<<"Database openned"<<opened;

    QJsonArray deviceList;
    if(opened){
        QSqlQuery query(os_db);
        query.prepare("SELECT [sourceId], [sourceName] FROM device_id");
        query.exec();
        qDebug()<<"Database openned";

        while (query.next())
        {
            QJsonObject obj = QJsonObject{{"id", query.value(0).toDouble()},{"name",query.value(1).toString()},};
            deviceList.push_back(obj);
        }
    }
    QJsonObject obj2={{"dataId",150},{"data",deviceList},};

    QJsonDocument doc = QJsonDocument(obj2);
    emit sendSensorDataToWebInterface(doc.toJson());
}

void DbReader::sendSensorList(QByteArray array)
{    
    QJsonDocument doc = QJsonDocument::fromJson(array);
    qDebug()<<doc.object();
    QJsonValue val= doc.object().value("data");

    QList<int>id;
    QJsonObject object;
    QJsonArray arr;

    if(val.isObject())
    {
        object=val.toObject();
        qDebug()<<"dataId:152 is JsonObject";
    }
    if(val.isArray())
    {
        arr=val.toArray();
        qDebug()<<"dataId:152 is JsonArray";
    }

    if((!object.empty()) || (!arr.empty()))
    {
        if(!object.empty())
        {
            id.append(object.value("id").toInt());
        }

        if(!arr.empty())
        {
            for(int i=0; i<arr.size();i++)
            {
                QJsonObject obj = arr[i].toObject();
                id.append(obj.value("id").toInt());
            }
        }
    }    

//    readSensorListFromConfig(id);
    readSensorListFromDb(id);
    //    QJsonArray _array;
    //    QStringList list;

    //    QSettings sensorDetails(qApp->applicationDirPath()+QString("/config_sensor.ini"),QSettings::IniFormat);
    //    sensorDetails.beginGroup("SensorDetails");
    //    int num = sensorDetails.beginReadArray("Sensors");

    //    for (int i = 0; i < num; i++) {
    //        sensorDetails.setArrayIndex(i);
    //        //        QString deviceName = sensorDetails.value("Device_Name").toString();
    //        int deviceId = sensorDetails.value("Device_Id").toInt();

    //        if(id.contains(deviceId))
    //        {
    //            int id = sensorDetails.value("Sensor_Id").toInt();
    //            QString sensorName= sensorDetails.value("Sensor_Name").toString();
    //            QJsonObject obj{
    //                {"id",id},
    //                {"name", sensorName},
    //            };
    //            _array.push_back(obj);
    //        }

    //        if(id.contains(-1))
    //        {
    //            int id = sensorDetails.value("Sensor_Id").toInt();
    //            QString sensorName= sensorDetails.value("Sensor_Name").toString();
    //            if(!list.contains(sensorName))
    //            {
    //                list.append(sensorName);
    //                QJsonObject obj{
    //                    {"id",id},
    //                    {"name", sensorName},
    //                };
    //                _array.push_back(obj);
    //            }
    //        }
    //    }
    //    sensorDetails.endArray();
    //    sensorDetails.endGroup();

    //    QJsonObject obj2{
    //        {"dataId",152},
    //        {"data", _array},
    //    };
    //    QJsonDocument doc2 =QJsonDocument(obj2);
    //    emit sendSensorDataToWebInterface(doc2.toJson());
    //    qDebug()<<obj2;
}

void DbReader::readSensorListFromConfig(QList<int> id)
{
    QJsonArray _array;
    QStringList list;

    QSettings sensorDetails(qApp->applicationDirPath()+QString("/config_sensor.ini"),QSettings::IniFormat);
    sensorDetails.beginGroup("SensorDetails");
    int num = sensorDetails.beginReadArray("Sensors");

    for (int i = 0; i < num; i++) {
        sensorDetails.setArrayIndex(i);
        //        QString deviceName = sensorDetails.value("Device_Name").toString();
        int deviceId = sensorDetails.value("Device_Id").toInt();

        if(id.contains(deviceId))
        {
            int id = sensorDetails.value("Sensor_Id").toInt();
            QString sensorName= sensorDetails.value("Sensor_Name").toString();
            QJsonObject obj{
                {"id",id},
                {"name", sensorName},
            };
            _array.push_back(obj);
        }

        if(id.contains(-1))
        {
            int id = sensorDetails.value("Sensor_Id").toInt();
            QString sensorName= sensorDetails.value("Sensor_Name").toString();
            if(!list.contains(sensorName))
            {
                list.append(sensorName);
                QJsonObject obj{
                    {"id",id},
                    {"name", sensorName},
                };
                _array.push_back(obj);
            }
        }
    }
    sensorDetails.endArray();
    sensorDetails.endGroup();

    QJsonObject obj2{
        {"dataId",152},
        {"data", _array},
    };
    QJsonDocument doc2 =QJsonDocument(obj2);
    qDebug()<<"communicator key = " << _communicator->transmitterKey;
    emit sendSensorDataToWebInterface(doc2.toJson());
    qDebug()<<obj2;
}

void DbReader::readSensorListFromDb(QList<int>id)
{
    QStringList list;
    QJsonArray sensorArr;
    if(id.contains(-1))
    {
        if(opened){
            QSqlQuery query(os_db);
            QString command = "SELECT [sensor_id], [sensor_name] FROM sensor_list";
            query.prepare(command);
            query.exec();
            while(query.next())
            {
                if(!list.contains(query.value(1).toString()))
                {
                    list.append(query.value(1).toString());
                QJsonObject obj{
                    {"id",query.value(0).toInt()},
                    { "name", query.value(1).toString()},
                };
                sensorArr.push_back(obj);
                }
            }
        }
        for(int i=0;i<list.size();i++)
        {
            qDebug()<<list[i];
        }
    }
    else
    {
        if(opened){
            QSqlQuery query(os_db);
            QString command = "SELECT [sensor_id], [sensor_name], [device_id] FROM sensor_list";
            query.prepare(command);
            query.exec();
            while(query.next())
            {
                if(id.contains(query.value(2).toInt()))
                {
                    qDebug()<<query.value(2).toInt();
                QJsonObject obj{
                    {"id",query.value(0).toInt()},
                    { "name", query.value(1).toString()},
                };
                sensorArr.push_back(obj);
                }
            }
        }
    }
    QJsonObject object{
        {"dataId",152},
        {"data",sensorArr},
    };
    QJsonDocument doc2 =QJsonDocument(object);
    emit sendSensorDataToWebInterface(doc2.toJson());
//    qDebug()<<object;

}



void DbReader::sendAllDetails(QByteArray array)
{
    QJsonDocument docs = QJsonDocument::fromJson(array);
    QJsonObject object = docs.object().value("data").toObject();
    QJsonValue val_1 = object.value("devices");

    QJsonObject sensorObj, deviceObj;
    QJsonArray sensorArray, deviceArray2;
    if(val_1.isObject())
    {
        deviceObj=val_1.toObject();
        qDebug()<<"Device is Json Obj";
    }
    else
    {
        deviceArray2 = val_1.toArray();
        qDebug()<<"Device is Json Arr";
    }
    QJsonValue val_2 = object.value("sensors");
    if(val_2.isObject())
    {
        sensorObj=val_2.toObject();
        qDebug()<<"Sensor is Json Obj";
    }
    else
    {
        sensorArray=val_2.toArray();
        qDebug()<<"Sensor is Json Arr";
    }

    QList<int>deviceList,sensorList;

    if(!deviceArray2.empty() || !deviceObj.empty())
    {
        if(!deviceArray2.empty())
            for(int i=0; i<deviceArray2.size();i++)
            {
                QJsonObject obj = deviceArray2[i].toObject();
                deviceList.append(obj.value("id").toInt());
            }
        if(!deviceObj.empty())
        {
            deviceList.append(deviceObj.value("id").toInt());
        }
    }

    if(!deviceList.isEmpty())
    {
        if (deviceList[0]==-1)
        {
            deviceList.clear();
            QSqlQuery query(os_db);
            query.prepare("SELECT [sourceId] FROM device_id");
            query.exec();
            int k=0;
            while (query.next())
            {
                deviceList.append(query.value(0).toInt());
                k++;
            }
        }
    }

    if(!sensorArray.empty() || !sensorObj.empty())
    {
        for(int i=0; i<sensorArray.size();i++)
        {
            QJsonObject obj = sensorArray[i].toObject();
            sensorList.append(obj.value("id").toInt());
        }
        if(!sensorObj.empty())
        {
            sensorList.append(sensorObj.value("id").toInt());
        }
    }

    if(!sensorList.isEmpty())
    {
        if (sensorList[0]==-1)
        {
            sensorList.clear();
            for(int i=0;i<6;i++)
            {
                sensorList.append(i+1);
            }
        }
    }

    QJsonObject time = object.value("timeStamps").toObject();
    qint64 start = time.value("start").toVariant().toLongLong();
    qint64 end = time.value("end").toVariant().toLongLong();
    int id;
    float value;


    QJsonArray sensorDataArray,timeStampArray,deviceArray;
    for(int i=0; i<deviceList.size();i++)
    {
        int size=0;
        if(opened){
            QSqlQuery query(os_db);
            QString command = "SELECT * FROM sensor_data WHERE device_id =" + QString::number(deviceList[i]) +" AND time_stamp>='"+QString::number(start)+"' AND time_stamp<='"+QString::number(end)+"'";
            query.prepare(command);
            query.exec();
            while(query.next())
            {
                float lat, lon;
                if(!query.value(4).isNull() && !query.value(5).isNull())
                {
                    lat=query.value(4).toFloat();
                    lon=query.value(5).toFloat();
                }else
                {
                    lat=query.value(6).toFloat();
                    lon=query.value(7).toFloat();
                }

                qint64 timestamp;
                timestamp=query.value(2).toLongLong();


                for(int j=0;j<sensorList.size();j++)
                {
                    id = sensorList[j];
                    value = query.value(sensorList[j]+7).toFloat();

                    QJsonObject obj{
                        {"id",id},
                        {"value",value},
                    };
                    sensorDataArray.append(obj);
                }
                QJsonObject object{
                    {"timeStamp",timestamp},
                    {"lat",lat},
                    {"lon",lon},
                    {"sensorDataArray",sensorDataArray},
                };
                timeStampArray.append(object);

                if(!sensorDataArray.isEmpty())
                {
                    while(sensorDataArray.count()){
                        sensorDataArray.pop_back();
                    }
                }
                size++;

                if(size>=100)
                {
                    qDebug()<<size;
                    sendBunchOfData(QJsonDocument(timeStampArray).toJson(), deviceList[i]);
                    size=0;
                    if(!timeStampArray.isEmpty())
                    {
                        while(timeStampArray.count()){
                            timeStampArray.pop_back();
                        }
                    }
                }
            }
            qDebug()<<size;
            QJsonObject deviceData{
                {"timeStampArray", timeStampArray},
            };
            if(!timeStampArray.isEmpty())
            {
                while(timeStampArray.count()){
                    timeStampArray.pop_back();
                }
            }
            QJsonObject deviceArr{
                {"deviceId",deviceList[i]},
                {"deviceData",deviceData},
            };
            deviceArray.append(deviceArr);
        }
    }
    QJsonObject data{
        {"deviceArray",deviceArray},
    };
    QJsonObject finalObject{
        {"dataId",154},
        {"data",data},
    };
    QJsonDocument doc= QJsonDocument(finalObject);
    emit sendSensorDataToWebInterface(doc.toJson());
    qDebug()<<finalObject;

    QJsonDocument docFinal= QJsonDocument(getRequestComplete());
    emit sendSensorDataToWebInterface(docFinal.toJson());
}

void DbReader::sendTrafficShips(QByteArray array)
{

}

void DbReader::sendBunchOfData(QByteArray ary, int deviceId)
{
    QJsonDocument doc = QJsonDocument::fromJson(ary);
    QJsonArray timeStampArray= doc.array();
    qDebug()<<QJsonDocument(timeStampArray);


    QJsonObject deviceData{
        {"timeStampArray", timeStampArray},
    };

    QJsonObject deviceArr{
        {"deviceId",deviceId},
        {"deviceData",deviceData},
    };
    QJsonArray deviceArray;
    deviceArray.append(deviceArr);


    QJsonObject data{
        {"deviceArray",deviceArray},
    };
    QJsonObject finalObject{
        {"dataId",154},
        {"data",data},
    };
    qDebug()<<"no problem here even here 3";
    QJsonDocument doc2= QJsonDocument(finalObject);
    emit sendSensorDataToWebInterface(doc2.toJson());
    qDebug()<<finalObject;
}

void DbReader::checkDataId(QByteArray array)
{       
    if(!opened)
    {
        qDebug()<<"Database has not been selected. Please select the database first";
    }

    qDebug()<<"DbReader::checkDataId() receives signal"<<array;
    QJsonDocument doc = QJsonDocument::fromJson(array);

    int dataId = doc.object().value("dataId").toInt();
    qDebug()<<dataId;

    if(dataId==0)
    {
        sendDeviceList();

    }else if (dataId == 151)
    {
        sendSensorList(array);

    }else if(dataId==153)
    {
        sendAllDetails(array);

    }else
    {
        qDebug()<<"AppId:MUH; DataId is wrong";
    }
}

void DbReader::setCommunicatorSettings(QString url, QString name)
{
    _communicator->setAmqpServer(url,name);
}

QJsonObject DbReader::getRequestComplete(){

    QJsonObject finalObject{
        {"dataId",155}
    };
    return finalObject;
}
