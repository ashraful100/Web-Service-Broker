
#include "xmlparser.h"
#include <QMessageBox>
#include <QMap>


XmlParser::XmlParser(QObject *parent) :
    QObject(parent)
{
    osDataSource =1; //Default XML
}
void XmlParser::parseXmlFromShs(QByteArray xml_array){
    if(osDataSource!=1)
        return;

    parseXML(xml_array);

}

void XmlParser::parseXML(QByteArray xml_array) {
    //qDebug()<<"XmlParser";
    QMap<QString, QString> ANSData;

    /* QDomDocument takes any QIODevice. as well as QString buffer*/
    QDomDocument doc("mydocument");
    if (!doc.setContent(xml_array))
    {
        return;
    }

    //Get the root element
    QDomElement docElem = doc.documentElement();


    QDomNodeList nodeListHeartBeat = docElem.elementsByTagName("heartbeat");

    for(int j = 0;j < nodeListHeartBeat.count(); j++)
    {
        QDomElement hbElem = nodeListHeartBeat.at(j).toElement();
        //get all data for the element, by looping through all child elements
        QDomNode hbEntries = hbElem.firstChild();
        while(!hbEntries.isNull()) {
            QDomElement hbElement = hbEntries.toElement();
            QString hbTagNam = hbElement.tagName();
            ANSData[hbTagNam] = hbElement.text();
            hbEntries = hbEntries.nextSibling();
        }

    }
    // get the node's interested in, this time only caring about person's
    QDomNodeList nodeList = docElem.elementsByTagName("elem");

    //Check each node one by one.
    for(int ii = 0;ii < nodeList.count(); ii++)
    {

        // get the current one as QDomElement
        QDomElement el = nodeList.at(ii).toElement();
        QString currentId;
        currentId = el.attribute("id"); // get and set the attribute ID
        QString currentNo = el.attribute("no");
        QString currentTs = el.attribute("ts");
        //get all data for the element, by looping through all child elements
        QDomNode pEntries = el.firstChild();

        while(!pEntries.isNull()) {
            QDomElement peData = pEntries.toElement();
            QString tagNam = peData.tagName();
            QString identifier;
            if(currentId.contains("TS_",Qt::CaseSensitive)){
                identifier =  QString("TRS_%1_%2_%3").arg(currentTs).arg(currentId).arg(tagNam);
            }
            else{
                identifier =  QString("%1_%2_%3").arg(currentId).arg(tagNam).arg(currentNo);
            }

            ANSData[identifier] = peData.text();
            pEntries = pEntries.nextSibling();

        }
    }
    //qDebug()<<ANSData;
    QString shipName = ANSData["OS_Name_name_0"];
    emit signalShipName(shipName);
    emit shsDataReceived(ANSData);


}


void XmlParser::setOsDataSource(int value)
{
    osDataSource = value;
}

