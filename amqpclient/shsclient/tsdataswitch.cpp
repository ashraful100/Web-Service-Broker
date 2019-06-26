#include "tsdataswitch.h"
#include <QDebug>

TsDataSwitch::TsDataSwitch(QObject *parent) : QObject(parent)
{
   targetId =2;
}

TsDataSwitch::~TsDataSwitch()
{

}

void TsDataSwitch::OnTsDataSourceChanged(int id)
{
    targetId =id;
}

void TsDataSwitch::onTsAisDataReceived(QMap<QString,QString> tsAisData){
    if(targetId == 1 || targetId == 3||targetId==4){ //AIS + TTM or TTM or AIS
        emit signalTsData(tsAisData);
    }
}

void TsDataSwitch::onTsShsDataReceived(QMap<QString,QString> tsShsData){
    if(targetId ==2){//SHS
        emit signalTsData(tsShsData);
    }
}
