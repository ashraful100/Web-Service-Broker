#include "amqpserversettings.h"
#include "ui_amqpserversettings.h"
#include <QSettings>
//!
//! \brief
//! \param parent
//!
AmqpServerSettings::AmqpServerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AmqpServerSettings)
{
    ui->setupUi(this);

    QSettings serverSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    serverSettings.beginGroup("ServerSettings");
    selectedServer = serverSettings.value("selectedServer").toInt();

    servers.clear();
    int numUrls = serverSettings.beginReadArray("Servers");

    for (int i = 0; i < numUrls; i++) {
        serverSettings.setArrayIndex(i);
        Server s;
        s.name = serverSettings.value("name").toString();
        s.url = serverSettings.value("url").toString();
        servers.append(s);
    }

    serverSettings.endArray();
    serverSettings.endGroup();

    //If no config file is in place (or no servers in it) take hard coded ones
    // and add them to the config file
    if (servers.size() == 0) {
        Server s;
        s.name = "CML";
        s.url = "amqp://cmlRqClient1:$CmlRqClient51036083@cml-mqrabbit.mls.tu-harburg.de:5672/cml";
        servers.append(s);
        s.name = "DSME";
        s.url = "amqp://dsmeDemoUser1:$Dsme2411@10.2.216.219:5672/dsme";
        servers.append(s);
        s.name = "Local";
        s.url = "amqp://localhost:local@localhost:5672/local";
        //        s.url = "amqp://guest:guest@localhost:5672";
        servers.append(s);
        s.name = "CML INT";
//        s.url = "amqp://10.157.8.142:cmlinternal@localhost:5672/cmlinternal";
        s.url="amqp://localhost:local@10.157.8.142:5672/cmlinternal";
        servers.append(s);
        s.name = "HIWI ROOM";
        s.url = "amqp://192.168.0.45:localhost@local:5672/local";
        servers.append(s);

        serverSettings.beginGroup("ServerSettings");
        serverSettings.beginWriteArray("Servers", servers.size());
        for (int i = 0; i < servers.size(); i++) {
            serverSettings.setArrayIndex(i);
            s = servers.at(i);
            serverSettings.setValue("name", s.name);
            serverSettings.setValue("url", s.url);
        }
        serverSettings.endArray();
        serverSettings.endGroup();
    }

    ui->cbSelectedServer->clear();

    for (int i = 0; i < servers.size(); i++) {
        ui->cbSelectedServer->addItem(servers.at(i).name);
    }

    if (selectedServer < ui->cbSelectedServer->count()) {
        ui->cbSelectedServer->setCurrentIndex(selectedServer);
        Server selected = servers.at(selectedServer);
        emit signalOnServerChanged(selected.url, selected.name);
    }
}

AmqpServerSettings::~AmqpServerSettings()
{
    delete ui;
}

int AmqpServerSettings::getSelectedServer() const
{
    return selectedServer;
}

void AmqpServerSettings::broadcastServerChoice()
{
    int index = ui->cbSelectedServer->currentIndex();
    Server selected =servers.at(index);
    emit signalOnServerChanged(selected.url, selected.name);
}

void AmqpServerSettings::on_cbSelectedServer_activated(int index)
{       
    Server selected =servers.at(index);
    selectedServer = index;    
    emit signalOnServerChanged(selected.url, selected.name);

    QSettings serverSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    serverSettings.beginGroup("ServerSettings");
    serverSettings.setValue("selectedServer",selectedServer);
    serverSettings.endGroup();
}
