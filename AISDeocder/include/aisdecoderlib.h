#ifndef AISDECODERLIB_H
#define AISDECODERLIB_H


#include <QtCore>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include "aisdecoderlib_global.h"
#include <src/ais/message_01.hpp>
#include <src/ais/message_05.hpp>
#include <src/ais/message.hpp>
#include <src/ais/ais.hpp>
#include <src/nmea/vdm.hpp>
#include <src/nmea/vdo.hpp>
#include <src/nmea/nmea.hpp>
#include <src/nmea/ais_helper.hpp>
#include <src/ais/name.hpp>
using namespace src;
using namespace ais;

class AISDECODERLIBSHARED_EXPORT AISDecoderLib:public QObject
{
    Q_OBJECT





public:

    typedef struct {

        int mmsi;
        navigation_status nav_status;
        double rot;
        double sog;
        bool position_accuracy =false;
        double lat;
        double lon;
        double cog;
        double hdg;
        QDateTime timestamp;
        maneuver_indicator_id maneuver_indicator;
        bool raim;
        int radio_status;

    }AisDynamicShip;

    typedef struct {

        int mmsi;
        int imo_number;
        QString callsign;
        QString shipname;
        int shiptype;
        double to_bow;
        double to_stern;
        double to_port;
        double to_starboard;
        epfd_fix_type epfd_fix;
        QDateTime eta;
        double draught;
        QString destination;
        data_terminal dte;

    }AisStaticShip;

    AISDecoderLib(QObject *parent = 0);
    bool getOs() const;
    void setOs(bool value);


    QString getUniqueId() const;
    void setUniqueId(const QString &value);

signals:
    void signalAisNMEAMessage(QByteArray aisMsg);
    void signalDecodedAisMessage(QMap<QString,QString> aisDecoded);

public slots:
    void generateMessage1(AisDynamicShip data);
    void generateMessage5(AisStaticShip data);
    void onAISMessageReceived(QByteArray aisRaw);
    void refresh();
private slots:



    void encodeNmeaOs(const ais::message &msg);
    void encodeNmeaTs(const ais::message &msg);
    void parseAis(const std::vector<std::string> received_strings);
private:
    bool os;

    std::vector<std::string> received_strings;
       QMap<QString,QString> shipData;

       QString msg8;

       QString uniqueId="";



};

#endif // AISDECODERLIB_H
