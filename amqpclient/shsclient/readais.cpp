#include "readais.h"

#include <QObject>
#include <QDebug>

ReadAis::ReadAis(QObject *parent) :
    QObject(parent)
{

}



void ReadAis::onDataReceived(QByteArray readDataAry)
{
    emit onReadAisSignal(readDataAry);
}



QString ReadAis::getAisMessage(){

    return aisNmeaMsg;

}
