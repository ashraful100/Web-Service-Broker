#include "dbserversettings.h"
#include "ui_dbserversettings.h"

DbServerSettings::DbServerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DbServerSettings)
{
    ui->setupUi(this);

    QSettings dbServerSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    dbServerSettings.beginGroup("DbServerSettings");
    selectedServer = dbServerSettings.value("selectedServer").toInt();

    dbServers.clear();
    int numUrls = dbServerSettings.beginReadArray("Servers");

    for (int i = 0; i < numUrls; i++) {
        dbServerSettings.setArrayIndex(i);
        DbServer s;
        s.host = dbServerSettings.value("host").toString();
        s.port = dbServerSettings.value("port").toString();
        s.database_os = dbServerSettings.value("database_os").toString();
        s.database_ts = dbServerSettings.value("database_ts").toString();
        s.DB_DRIVER = dbServerSettings.value("DB_DRIVER").toString();
        s.DB_ID = dbServerSettings.value("DB_ID").toString();
        dbServers.append(s);
    }

    dbServerSettings.endArray();
    dbServerSettings.endGroup();

    //If no config file is in place (or no servers in it) take hard coded ones
    // and add them to the config file
    if (dbServers.size() == 0) {
        DbServer s;

        s.host = "Local_MSACCESS";
        s.port = "1433";
        s.database_os ="EMSN_DATA";
        s.database_ts ="EMSN_DATA";
        s.DB_DRIVER = "QODBC3";
        s.DB_ID = "EMSN_Local";
        dbServers.append(s);

        s.host = "CML-DATA-SV";
        s.port = "1433";
        s.database_os ="SENSOR_TERMINAL";
        s.database_ts ="TRAFFIC_SHIP_DATA";
        s.DB_DRIVER = "QODBC3";
        s.DB_ID = "EMSN_CML_MSQL";
        dbServers.append(s);

//        s.host = "CML-DATA-SV-TRAFFIC";
//        s.port = "1433";
//        s.database ="TRAFFIC_SHIP_DATA";
//        s.DB_DRIVER = "QODBC3";
//        s.DB_ID = "EMSN_CML_AIS_MSQL";
//        dbServers.append(s);

        dbServerSettings.beginGroup("DbServerSettings");
        dbServerSettings.beginWriteArray("Servers", dbServers.size());
        for (int i = 0; i < dbServers.size(); i++) {
            dbServerSettings.setArrayIndex(i);
            s = dbServers.at(i);
            dbServerSettings.setValue("host", s.host);
            dbServerSettings.setValue("port", s.port);
            dbServerSettings.setValue("database_os", s.database_os);
            dbServerSettings.setValue("database_ts", s.database_ts);
            dbServerSettings.setValue("DB_DRIVER", s.DB_DRIVER);
            dbServerSettings.setValue("DB_ID", s.DB_ID);
        }
        dbServerSettings.endArray();
        dbServerSettings.endGroup();
    }

    ui->cbDbSelectedServer->clear();

    for (int i = 0; i < dbServers.size(); i++) {
        ui->cbDbSelectedServer->addItem(dbServers.at(i).host);
    }

    if (selectedServer < ui->cbDbSelectedServer->count()) {
        ui->cbDbSelectedServer->setCurrentIndex(selectedServer);
        //        DbServer selected = dbServers.at(selectedServer);
        selectDatabase(selectedServer);
        emit signalOnServerChanged(db_Id, DB_CONNECTION_SETTINGS_OS, DB_CONNECTION_SETTINGS_TS);
    }
}

DbServerSettings::~DbServerSettings()
{
    delete ui;
}

int DbServerSettings::getSelectedServer() const
{
    return selectedServer;
}

void DbServerSettings::on_cbDbSelectedServer_activated(int index)
{
    //    DbServer selected =dbServers.at(index);
    //    selectedServer = index;
    //    emit signalOnServerChanged(selected.url, selected.name);
    //    qDebug()<<selected.url;

    //    QSettings dbServerSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    //    dbServerSettings.beginGroup("DbServerSettings");
    //    dbServerSettings.setValue("selectedServer",selectedServer);
    //    dbServerSettings.endGroup();

    selectedServer = index;
    selectDatabase(index);
    emit signalOnServerChanged(db_Id,DB_CONNECTION_SETTINGS_OS,DB_CONNECTION_SETTINGS_TS);
    QSettings dbServerSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    dbServerSettings.beginGroup("DbServerSettings");
    dbServerSettings.setValue("selectedServer",selectedServer);
    dbServerSettings.endGroup();
//    qDebug()<<DB_CONNECTION_SETTINGS_OS;
//    qDebug()<<DB_CONNECTION_SETTINGS_TS;
}

void DbServerSettings::selectDatabase(int db)
{
    DbServer selected =dbServers.at(db);
    QString localOwnShipPath,localTrafficShipPath;
    switch ((DB_TYPE)db) {
    case Local_MSACCESS:
        localOwnShipPath = QString("%1/%2").arg(qApp->applicationDirPath()).arg("sensor_data_local.mdb");
        localTrafficShipPath = QString("%1/%2").arg(qApp->applicationDirPath()).arg("traffic_ship_data.mdb");
        DB_CONNECTION_SETTINGS_OS = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + localOwnShipPath;
        DB_CONNECTION_SETTINGS_TS = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + localTrafficShipPath;
        db_Id=0;
        break;

    case CML_MSQL:
        DB_CONNECTION_SETTINGS_OS = "DRIVER={SQL Server};Server=" +selected.host+";Port="+selected.port+";Database="+selected.database_os+";Integrated Security=True;";
        DB_CONNECTION_SETTINGS_TS = "DRIVER={SQL Server};Server=" +selected.host+";Port="+selected.port+";Database="+selected.database_ts+";Integrated Security=True;";
        db_Id=1;
        break;    
    }
}
