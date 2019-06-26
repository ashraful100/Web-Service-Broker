#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QObject>
#include <QSqlDatabase>
#include <QTime>
#include <QSqlQuery>
#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include <QByteArray>
#include <QMap>

#include "shipmotionentity.h"
#include "simulationinformation.h"
#include "navlightsandshapes.h"

enum DB_TYPE{

    CML_DATA_SV =0,
    CML_RS_SIM,
    CML_Local
};

class DatabaseInterface : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseInterface(QObject *parent = 0);
    ~DatabaseInterface();

    //store and return generated id if stored successfully..
    int storeNewSimulationInstance(QDateTime startTime, QString shortName, QString description);
    void storeEndTimeOfSimulationInstance(int simulationId, QDateTime endTime, bool success);

    //simulation startTime must be given to calculate time step for this entry, instead of retreiving it from database..
    bool storeShipMotionData(ShipMotionEntity *shipEntity, QDateTime simulationStartTime);

    void storeShipMotionData(QMap<QString, QString> &fixedDatumMap, int simulationId, QDateTime simulationStartTime);

    void storeAISData(QString aisData, int simulationId, int siteId, int appId, int entityId, int exerciseId);
    void storeNavLightsAndShapes(int simulationId, int siteId, int appId, int entityId, int exerciseId, int light, int shape);


    QList<int> getSimulationIds();
    QList<int> getSiteIds();
    QList<int> getApplicationIds();
    QList<int> getEntityIds();
    QList<ShipMotionEntity *> getShipMotionData(int simId, int appId=-1, int siteId=-1, int entityId=-1);



signals:

    void signalRecordingFinished(bool success,QString message);

public slots:

    QMap<int,SimulationInformation> getSimulationIdWithData();
    void selectDataBase(int db);
private:
    void closeDB();
    bool openDB();


     QString DB_DRIVER = "QODBC3";
     QString DB_ID = "GRIB";
     QSqlDatabase db;
     bool opened = false;
     //QList<NavLightsAndShapes*> navLightsAndShapes;

     QString host = "CML-DATA-SV";
     QString port = "1433";
     QString database ="EMSN_DATA";
     QString DB_CONNECTION_SETTINGS;

};

#endif // DATABASEINTERFACE_H
