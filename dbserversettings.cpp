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
        s.name = dbServerSettings.value("name").toString();
        s.url = dbServerSettings.value("url").toString();
        dbServers.append(s);
    }

    dbServerSettings.endArray();
    dbServerSettings.endGroup();

    //If no config file is in place (or no servers in it) take hard coded ones
    // and add them to the config file
    if (dbServers.size() == 0) {
        DbServer s;

        s.name = "Local";
        s.url = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + QString("C:\\workspace\\git_version\\web-sbi\\member.mdb");
        dbServers.append(s);

        dbServerSettings.beginGroup("DbServerSettings");
        dbServerSettings.beginWriteArray("Servers", dbServers.size());
        for (int i = 0; i < dbServers.size(); i++) {
            dbServerSettings.setArrayIndex(i);
            s = dbServers.at(i);
            dbServerSettings.setValue("name", s.name);
            dbServerSettings.setValue("url", s.url);
        }
        dbServerSettings.endArray();
        dbServerSettings.endGroup();
    }

    ui->cbDbSelectedServer->clear();

    for (int i = 0; i < dbServers.size(); i++) {
        ui->cbDbSelectedServer->addItem(dbServers.at(i).name);
    }

    if (selectedServer < ui->cbDbSelectedServer->count()) {
        ui->cbDbSelectedServer->setCurrentIndex(selectedServer);
        DbServer selected = dbServers.at(selectedServer);
        emit signalOnServerChanged(selected.url, selected.name);
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

void DbServerSettings::broadcastServerChoice()
{
    int index = ui->cbDbSelectedServer->currentIndex();
    DbServer selected =dbServers.at(index);
    emit signalOnServerChanged(selected.url, selected.name);
}

void DbServerSettings::on_cbDbSelectedServer_activated(int index)
{
    DbServer selected =dbServers.at(index);
    selectedServer = index;
    emit signalOnServerChanged(selected.url, selected.name);
    qDebug()<<selected.url;

    QSettings dbServerSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    dbServerSettings.beginGroup("DbServerSettings");
    dbServerSettings.setValue("selectedServer",selectedServer);
    dbServerSettings.endGroup();   
}
