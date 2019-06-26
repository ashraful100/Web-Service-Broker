#include "databasesettings.h"

DatabaseSettings::DatabaseSettings(QObject *parent) : QObject(parent)
{
//    openDB();
}

DatabaseSettings::~DatabaseSettings()
{
    closeDB();

}

bool DatabaseSettings::openDB()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + QString("T:\\Transfer\\STNS HiWi-Projektordner\\Ashraful\\WEB-SPI-Uncommited\\web-sbi\\member.mdb"));

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


    //    if(opened){
    //        db.close();
    //        opened =false;
    //    }
    //    bool success = false;

    //    if(!opened) {

    //        QString host = "CML-DATA-SV";
    //        QString port = "1433";
    //        QString database ="EMSN_DATA";
    //        QString DB_DRIVER = "QODBC3";
    //        QString DB_ID = "EMSN_DATA_SV";

    //        QString DB_CONNECTION_SETTINGS = "DRIVER={SQL Server};Server=" +host+";Port="+port+";Database="+database+";Integrated Security=True;";


    //        db = QSqlDatabase::addDatabase(DB_DRIVER, DB_ID);
    //        qDebug() << DB_CONNECTION_SETTINGS;
    //        db.setDatabaseName(DB_CONNECTION_SETTINGS);

    //        // open connection
    //        success = db.open();
    //        if(success){
    //            qDebug("DB connection succeeded!");
    //            opened = true;
    //        }else{
    //            qDebug("DB connection failed!");
    //            opened = false;
    //        }
    //    }
    //    return success;
}

void DatabaseSettings::closeDB()
{
    db.close() ;
    qDebug()<<"Db Closed Successfully!!";

}

void DatabaseSettings::importData()
{
    if(opened){
        QSqlQuery query(db);
        query.prepare("SELECT TOP 1000 [id] ,[node_id] ,[timestamp] ,[date_time], [lat], [lon], [sensor_1], [sensor_2], [sensor_3], [sensor_4], [course], [device_id] FROM sensor_data");
        query.exec();
        while (query.next())
        {
            qDebug()<<"Data "<<query.value(0);
            qDebug()<< query.value(0)<<query.value(1)<<query.value(2)<<query.value(3);
            qDebug()<< query.value(4)<<query.value(5)<<query.value(6)<<query.value(7);
            qDebug()<< query.value(8)<<query.value(9)<<query.value(10)<<query.value(11);


            //            ui->textEdit->append(QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12")
            //                                 .arg(query.value(0).toString())
            //                                 .arg(query.value(1).toString())
            //                                 .arg(query.value(2).toString())
            //                                 .arg(query.value(3).toString())
            //                                 .arg(query.value(4).toString())
            //                                 .arg(query.value(5).toString())
            //                                 .arg(query.value(6).toString())
            //                                 .arg(query.value(7).toString())
            //                                 .arg(query.value(8).toString())
            //                                 .arg(query.value(9).toString())
            //                                 .arg(query.value(10).toString())
            //                                 .arg(query.value(11).toString())
        }
    }
}

void DatabaseSettings::storeData()
{
    unsigned int m=1;
    if(opened){
        QSqlQuery query(db);

        for(unsigned int i =0;i<50;i++)
        {
            query.prepare("INSERT INTO sensor_data (node_id, [timestamp], date_time, lat, lon, sensor_1, sensor_2, sensor_3, sensor_4, course, device_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query.bindValue(0, i);
            query.bindValue(1, i*10);
            query.bindValue(2, QDateTime::currentDateTime().toString("dd MM yyyy hh:mm:ss"));
            query.bindValue(3, 12);
            query.bindValue(4, 100);
            query.bindValue(5, 12);
            query.bindValue(6, 100);
            query.bindValue(7, 12);
            query.bindValue(8, 100);
            query.bindValue(9, 12);
            query.bindValue(10, 100);

            query.exec();
            db.commit();
            qDebug()<<m<<"row imported.";
            m++;
        }
    }
}

void DatabaseSettings::selectDB()
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


