#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
//#include <QDomElement>
#include <QMap>
#include <QDebug>

//!
//! \brief The XmlParser class handles the xml ship data received by the SHS Client and parses it to data maps. The component splits the xml data into data maps for OwnShip and individual trafficships separately.
//!\ingroup shsclient
//!
class XmlParser : public QObject
{
    Q_OBJECT
public:
    explicit XmlParser(QObject *parent = 0);



signals:

    void shsDataReceived(QMap<QString,QString> shsData);

    void signalShipName(QString &name);

public slots:

    void parseXML(QByteArray xml_array);

    void setOsDataSource(int value);

    void parseXmlFromShs(QByteArray xml_array);
protected slots:

private:

  int osDataSource;
    };

#endif // XMLPARSER_H
