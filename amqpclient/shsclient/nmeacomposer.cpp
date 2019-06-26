#include "nmeacomposer.h"
#include <QDebug>

NmeaComposer::NmeaComposer(QObject *parent) :
    QObject(parent)
{
}


void NmeaComposer::write_nmea_eot(double eotValue,int eotId)
{
    qDebug() << "setEot in composer" << eotValue;
    QString eotNmea =QString("$--EOT,%1,%2,A,*hh<CR><LF>\r\n").arg(eotId).arg(eotValue);
    emit sendControlMsg(eotNmea);
}

//!
//! \brief converts the eotValue to a nmea command and emits it as a Control Message
//! \param eotValue EOT value to be converted
//!
void NmeaComposer::write_nmea_eot(double eotValue)
{
//    qDebug() << "setEot in composer" << eotValue;
    QString eotNmea =QString("$--EOT,%1,%2,A,*hh<CR><LF>\r\n").arg(3).arg(eotValue);
    emit sendControlMsg(eotNmea);
}

void NmeaComposer::sendVirtualOsLocation(QString &name,double lat, double lon)
{
    QString virtualOsLocation =QString("$%1,%2,%3,*5E<CR><LF>\r\n").arg(name).arg(lat).arg(lon);
    emit sendControlMsg(virtualOsLocation);

}
/*-------------------------------------------------------------------------------------------------------Compose/Write NMEA:Rudder*/
//!
//! \brief converts the rudder values to a nmea command and emits it as a Control Message
//! \param rudderValue_strbrd starboard rudder command to be converted
//! \param rudderValue_port port rudder command to be converted
//!
void NmeaComposer::write_nmea_rudder(double rudderValue_strbrd, double rudderValue_port)
{
    QString rudderNmea =QString("$WIRSA,%1,A,%2,A,*5E<CR><LF>\r\n").arg(QString::number(rudderValue_strbrd,'f',0)).arg(QString::number(rudderValue_port,'f',0));
    emit sendControlMsg(rudderNmea);

}
/*---------------------------------------------------------------------------------------------------------Compose/Write NMEA:Course*/
void NmeaComposer::write_nmea_course(double courseValue)
{
//    qDebug() << "Cmd-Course = " << courseValue;
    QString courseNmea = QString("$--VTG,%1,T,,,,,,,,*hh<CR><LF>\r\n").arg(courseValue);
    emit sendControlMsg(courseNmea);


}
/*---------------------------------------------------------------------------------------------------------Compose/Write NMEA:Course*/
void NmeaComposer::write_nmea_sog(double sogValue)
{
    QString courseNmea = QString("$--VTG,,T,,M,%1,N,,,,*hh<CR><LF>\r\n").arg(sogValue);
    emit sendControlMsg(courseNmea);


}
/*---------------------------------------------------------------------------------------------------------Compose/Write NMEA:Course*/
void NmeaComposer::write_nmea_courseAndSog(double courseValue,double sog)
{
//    qDebug() << "Cmd-Course = " << courseValue<< "Cmd-sog = " << sog;

    QString courseNmea = QString("$--VTG,%1,T,,M,%2,N,,,,*hh<CR><LF>\r\n").arg(courseValue).arg(sog);
    emit sendControlMsg(courseNmea);


}
/*---------------------------------------------------------------------------------------------------------Compose/Write NMEA:Thruster*/
void NmeaComposer::write_nmea_thruster(double thruster, int thrusterId)
{

    QString thrusterNmea = QString("$PATLTHR,%1,%2,A*00<CR><LF>\r\n").arg(thrusterId).arg(thruster);
    emit sendControlMsg(thrusterNmea);

}

/*---------------------------------------------------------------------------------------------------------Compose/Write NMEA:AcControl*/
void NmeaComposer::write_nmea_ac_control(bool active){

    QString acNmea = QString("$---TP,%1,A*00<CR><LF>\r\n").arg(active);
    emit sendControlMsg(acNmea);
}

