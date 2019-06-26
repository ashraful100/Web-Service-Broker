#include "sensorinterface.h"
#include "ui_sensorinterface.h"

SensorInterface::SensorInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorInterface)
{
    ui->setupUi(this);
    //    dbreader = new DbReader(this);
    //    connect(dbreader, SIGNAL(sendSensorDataToWebInterface(QByteArray)),this,SLOT(checkReceivedJson(QByteArray)));
}

SensorInterface::~SensorInterface()
{  
    delete ui;
}

void SensorInterface::checkDataID(QByteArray array)
{            
    QJsonDocument doc = QJsonDocument::fromJson(array);
    int dataId = doc.object().value("dataId").toInt();
    //    if(dataId == 0 || dataId == 151 || dataId == 153)
    //        dbreader->checkDataId(array);
}

void SensorInterface::checkReceivedJson(QByteArray array)
{
    qDebug()<<"Problem is not in SENSOR_INTERFACE "<<QJsonDocument::fromJson(array);

    if(array.data())
        qDebug()<<"Problem is not in SENSOR_INTERFACE "<<QJsonDocument::fromJson(array);
    emit sendSensorData(array);
}

void SensorInterface::receiveServerUrl(int id,QString serverUrl)
{
    if(serverUrl.data())
    {
        setServer = true;
    }
    else
    {
        setServer = false;
    }
    if(setServer)
    {
        //        bool connected = dbreader->openDB(serverUrl);
        //        if(connected)
        //        {
        //            qDebug()<<"Database is connected.";
        //        }
        //        else
        //        {
        //            qDebug()<<"Database connection error.";
        //        }
        //        emit connectionStatus(connected);
    }
}

void SensorInterface::onWebDataRecived(QByteArray array)
{
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QString userId=doc.object().value("userId").toString();    

    if(userId.isEmpty())
    {
        userId=user_id;
    }
    else
    {
        qDebug()<<"User Id is not empty";
    }

    DbReader* dbReader;
    if(dbReaderMap.contains(userId)){

        dbReader= getDbReader(userId);        
    }else{
        user_id=userId;
        dbReader =new DbReader(ownShip,trafficShip,userId);
        qDebug()<<"Here 1";

        //connect(this,SIGNAL(modifiedReqFromWeb(QString userId, QByteArray req)))
        setDbReader(dbReader,userId);
        dbReader->setCommunicatorSettings(url_Communicator,name_Communicator);
//        db.close();
    }

    dbReader->checkDataId(array);
}

void SensorInterface::checkDatabaseConnection(int id, QString osUrl, QString tsUrl)
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
        ownShip=osUrl;
        trafficShip=tsUrl;
    }
    else
    {
        opened =false;
        qDebug()<<"Error 1: "<<ownShip_db.lastError().text()<<"Error 2: "<<trafficShip_db.lastError().text();;
    }

    emit connectionStatus(opened);
}

void SensorInterface::setCommunicatorSettings(QString url, QString name)
{
    url_Communicator=url;
    name_Communicator=name;
    QMapIterator<QString,DbReader*>dbReaderIterator(dbReaderMap);
    while(dbReaderIterator.hasNext()){
        dbReaderIterator.next();
        QString userId=dbReaderIterator.key();
        getDbReader(userId)->setCommunicatorSettings(url_Communicator,name_Communicator);
    }
}

void SensorInterface::setDbReader(DbReader* dbreader, QString userId)
{
    dbReaderMap[userId]= dbreader;
}

DbReader *SensorInterface::getDbReader(QString userId)
{
    return dbReaderMap[userId];
}

