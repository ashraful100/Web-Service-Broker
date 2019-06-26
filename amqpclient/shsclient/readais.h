#ifndef READAIS_H
#define READAIS_H

#include <QApplication>
#include <QObject>

//!
//! \brief The ReadAis class reads the AIS data from Ship Handling Simulator
//! \ingroup shsclient
//!
class ReadAis : public QObject
{
    Q_OBJECT

public:

    explicit ReadAis(QObject *parent = 0);

signals:

    void onReadAisSignal(QByteArray);

public slots:

    QString getAisMessage();

private slots:

    void onDataReceived(QByteArray readDataAry);   

private:

     QString aisNmeaMsg;

};

#endif // READAIS_H
