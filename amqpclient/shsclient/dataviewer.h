#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>

namespace Ui {
class DataViewer;
}

//!
//! \brief The DataViewer class enables viewing of incoming/outgoing data in ConnectionManager
//! \ingroup shsclient
//!
class DataViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DataViewer(QWidget *parent = 0);
    ~DataViewer();

public slots:
    void clearData();
    void setData(QByteArray data);
    void setDataStr(QString data);
    void setDataType(QString dataName, QString dataType);
    void setClearTimer(bool useTimer, uint clearIntervalInSec);
    void setCaLogData(QString data, int color=-1);
    void setIdentifier(QString appId, int shsId);
private slots:
    void on_pbLog_clicked(bool checked);

    void on_pbCancel_clicked();

    void on_pbStart_clicked();

    void on_pbBrowse_clicked();

    void logDirPath();
private:
    Ui::DataViewer *ui;
    QTimer *timer;
    QString dataName;
    QString appId;
    int shsId;
    QString logDir;
    QFile logFile;
    QTextStream logStream;
    bool loggingOn;
};

#endif // DATAVIEWER_H
