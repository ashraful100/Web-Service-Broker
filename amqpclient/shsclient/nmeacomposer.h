#ifndef NMEACOMPOSER_H
#define NMEACOMPOSER_H

#include <QObject>

//!
//! \brief The NmeaComposer class converts the ship commands received from the user to the corresponding NMEA format.
//! \ingroup shsclient
//!
//! The NmeaComposer class converts the ship commands received from the user to the corresponding NMEA format readable by the ship handling simulator (SHS). The following ship commands are handled.
//! 1. Course Command (VTG)
//! 2. Engine Order Telegraph (EOT)
//! 3. Rudder (RSA)
//!
class NmeaComposer : public QObject
{
    Q_OBJECT
public:
    explicit NmeaComposer(QObject *parent = 0);

signals:
   void  sendControlMsg(QString nmeaMsg);  

public slots:

    void write_nmea_eot(double eotValue, int eotId);
    void write_nmea_rudder(double rudderValue_strbrd, double rudderValue_port);
    void write_nmea_course(double courseValue);
    void write_nmea_sog(double sogValue);
    void write_nmea_thruster(double thruster, int thrusterId);
    void write_nmea_eot(double eotValue);
    void write_nmea_ac_control(bool active);
    void sendVirtualOsLocation(QString &name, double lat, double lon);

    void write_nmea_courseAndSog(double courseValue, double sog);
};

#endif // NMEACOMPOSER_H
