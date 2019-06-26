#include "dataviewer.h"
#include "ui_dataviewer.h"

DataViewer::DataViewer( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);



    timer =new QTimer();
    connect(timer,SIGNAL(timeout()),ui->teDataViewer,SLOT(clear()));
    //timer->start(100000);

    ui->frLogSettings->setHidden(true);
    loggingOn =false;
    ui->leLogPath->setText("Stopped");
    ui->lbLogStatus->setText("Logging:");
}

DataViewer::~DataViewer()
{
    delete ui;
    delete timer;
}

void DataViewer::clearData()
{
    ui->teDataViewer->clear();
}
void DataViewer::setIdentifier(QString appId,int shsId){
    this->appId =appId;
    this->shsId=shsId;
}

void DataViewer::setClearTimer(bool useTimer, uint clearIntervalInSec){

    if(useTimer){
        timer->start(clearIntervalInSec*1000);
    }
    else {
        timer->stop();
    }
}

void DataViewer::setData(QByteArray data){
    clearData();
    ui->teDataViewer->append(QString(data));
    if(loggingOn){
        logStream << QDateTime::currentDateTime().toString("ddMMyyyyhhmmss;")<< data<<endl;
    }
}

void DataViewer::setDataStr(QString data){
    clearData();
    ui->teDataViewer->append(data);
    if(loggingOn){
        logStream << QDateTime::currentDateTime().toString("ddMMyyyyhhmmss;")<< data<<endl;
    }
}

void DataViewer::setCaLogData(QString data, int color){

    switch (color) {
    case 0:
        ui->teDataViewer->setTextColor("grey");
        break;
    case 1:
        ui->teDataViewer->setTextColor(QColor(231, 76, 60)); //Red
        break;
    case 2:
        ui->teDataViewer->setTextColor(QColor(46, 204, 113)); //Green
        break;
    case 3:
        ui->teDataViewer->setTextColor(QColor(241, 196, 15)); //Yellow
        break;
    default:
        ui->teDataViewer->setTextColor("grey");
        break;
    }

    QString now =QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    ui->teDataViewer->append(now);
    ui->teDataViewer->append("----------------------------");
    ui->teDataViewer->append(data);
    ui->teDataViewer->append("");
    if(loggingOn){
        logStream << QDateTime::currentDateTime().toString("ddMMyyyyhhmmss;")<< data<<endl;
    }

}

void DataViewer::setDataType(QString dataName,QString dataType){
    ui->lbDataName->setText(dataName);
    ui->lbDataType->setText(dataType);
    this->dataName =dataName;

    QSettings logSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    logSettings.beginGroup("LogSettings");
    logSettings.beginReadArray(QString("App:%1/Ship:%2/Data:%3/").arg(appId).arg(shsId).arg(dataName));
    logDir =logSettings.value("logDir").toString();
    logSettings.endArray();
    logSettings.endGroup();
    ui->leLogFile->setText(logDir);

}
void DataViewer::on_pbLog_clicked(bool checked)
{
    if(checked){
        ui->pbLog->setText("Stop Log");
        ui->frLogSettings->setHidden(false);
         ui->lbLogStatus->setText("Logging to:");
    }
    else {
        ui->pbLog->setText("Log");
        ui->frLogSettings->setHidden(true);
        //Stop Logging
        loggingOn =false;
        logFile.close();
        ui->leLogPath->setText("Stopped");
        ui->lbLogStatus->setText("Logging:");
    }
}

void DataViewer::on_pbCancel_clicked()
{
    ui->frLogSettings->setHidden(true);
}

void DataViewer::on_pbStart_clicked()
{
    ui->frLogSettings->setHidden(true);

    QDir dir(logDir);
    if(dir.exists()==false){
        QMessageBox::warning( this, tr( "Data Logging" ), tr( "No Log Directory Chosen.\n Please select a location for the log file" ) );
        return;
    }
    QString file = QString("%1_Ship%2_%3_Log_%4.log")
            .arg(appId) //App Id
            .arg(shsId)  //Shs Id
            .arg(dataName) //Data Name
            .arg(QDateTime::currentDateTime().toString("ddMMyyyy_hh_mm")); //Time Stamp
   logFile.setFileName(logDir+"/"+file);


    ui->leLogPath->setText(logDir+"/"+file);

    if (logFile.open(QFile::WriteOnly | QFile::Truncate|QFile::Text)) {
        logStream.setDevice(&logFile);

        logStream << QString("Begin Log ; Application:%1; Data Name:%2; Ship Id:%3; Time Start:%4")
                     .arg(appId)
                     .arg(dataName)
                     .arg(shsId)
                     .arg(QDateTime::currentDateTime().toString("dd MM yyyy hh:mm:ss"))<<endl;

    }
    //Start Logging
    loggingOn =true;
}

void DataViewer::on_pbBrowse_clicked()
{
    logDirPath();
}

void DataViewer::logDirPath()
{

    logDir = QFileDialog::getExistingDirectory(this, tr("Choose Log Directory"),
                                               qApp->applicationDirPath(),
                                               QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir dir(logDir);
    if(dir.exists()==false){
        QMessageBox::warning( this, tr( "Data Logging" ), tr( "Log Directory not selected" ) );
        return;
    }
    QSettings logSettings(qApp->applicationDirPath()+QString("/config.ini"),QSettings::IniFormat);
    logSettings.beginGroup("LogSettings");
    logSettings.beginWriteArray(QString("App:%1/Ship:%2/Data:%3/").arg(appId).arg(shsId).arg(dataName));
    logSettings.setValue("logDir",logDir);
    logSettings.endArray();
    logSettings.endGroup();

    ui->leLogFile->setText(logDir);


}
