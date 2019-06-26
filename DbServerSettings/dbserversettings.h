#ifndef DBSERVERSETTINGS_H
#define DBSERVERSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QDebug>
#include <QDir>

namespace Ui {
class DbServerSettings;
}

struct DbServer {
    QString host;
    QString port="1433";
    QString database_os;
    QString database_ts;
    QString DB_DRIVER;
    QString DB_ID;
};

enum DB_TYPE{
    Local_MSACCESS =0,
    CML_MSQL,    
};

class DbServerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit DbServerSettings(QWidget *parent = 0);
    ~DbServerSettings();

    int getSelectedServer() const;

signals:
    void signalOnServerChanged(int id, QString osServerUrl, QString tsServerUrl);

private slots:

    void on_cbDbSelectedServer_activated(int index);

private:
    Ui::DbServerSettings *ui;
    QList<DbServer> dbServers;
    int selectedServer;
    void selectDatabase(int db);

    QString DB_DRIVER = "QODBC3";
    QString DB_ID = "GRIB";
    QString host = "CML-DATA-SV";
    QString port = "1433";
    QString database ="EMSN_DATA";
    QString DB_CONNECTION_SETTINGS_OS,DB_CONNECTION_SETTINGS_TS;
    int db_Id;
};

#endif // DBSERVERSETTINGS_H
