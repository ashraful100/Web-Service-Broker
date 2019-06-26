#ifndef RADAR_H
#define RADAR_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QMainWindow>
#include <QTimer>
#include <QObject>
#include "readgeneral.h"

namespace Ui {
class ReadTtm;
}

//!
//! \brief The ReadTtm class reads the Radar (TTM) data from Ship Handling Simulator
//! \ingroup shsclient
//!
//! *'*
class ReadTtm : public QMainWindow
{
    Q_OBJECT

public:

    explicit ReadTtm(QWidget *parent = 0);
    ~ReadTtm();
    QString label_test;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


public slots:    

    void setOsData(QMap<QString,QString> ownship);
    QStringList getTtmNmeamsg();

signals:

   void sendTtmToAmqp(QByteArray);
   void sendTtmTargetList(QList<QStringList> targetList);
private slots:

    void createGraphicView_radar();
    void onPortReadTTM(QByteArray);       
    void refreshList();  void parseTtmMsg(QStringList, QString ttmNmeaMsg);    
    void drawRadarBg();

    void draw();
    void on_comboBox_activated(int index);

    void parseTargets();
private:

    Ui::ReadTtm *ui;
    QGraphicsScene *scene_radar;
    QGraphicsPixmapItem* item_target;
    QGraphicsTextItem* itemHdgText;
    QList<QStringList> targetList;
    QTimer* targetListRefreshTimer;
    int amqpSendCounter;

    QGraphicsEllipseItem *radar_dial;
    QGraphicsLineItem *scanner;
    QGraphicsLineItem *tickMinor;
    QStringList listTtmNmeaMsgs;
    QMutex mutex;  
    QRectF radarBoundRect;

    double os_hdg,tickLngtMajor,tickLngtMinor,radarDialRadius;
    QMap<QString, QString> ownship;
    int range;
};

#endif // RADAR_H
