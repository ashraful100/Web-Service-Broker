#include "radar.h"
#include "ui_radar.h"
#include <QGraphicsView>
#include <QImage>
#include <QFont>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QDateTime>

/*--------------------------------------------------------------------------------------------------------Constructor*/
ReadTtm::ReadTtm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReadTtm)
{
    ui->setupUi(this);



    /*Variable init*/
    amqpSendCounter = 0;
    os_hdg=0;
    radarDialRadius =240;
    tickLngtMajor =8;
    tickLngtMinor =4;

    /*Graphics view for radar*/
    createGraphicView_radar();

    targetListRefreshTimer = new QTimer(this);
    targetListRefreshTimer->setInterval(60000);
    connect(targetListRefreshTimer, SIGNAL(timeout()), SLOT(refreshList()));
    //targetListRefreshTimer->start();
}
/*----------------------------------------------------------------------------------------------------------destructor*/
ReadTtm::~ReadTtm()
{
    delete ui;
    delete targetListRefreshTimer;

}


/*-----------------------------------------------------------------------------------------------------------Read TTM*/
void ReadTtm::onPortReadTTM(QByteArray read_ttm_ary)
{
    // DSMe While sending block messages

    QStringList blockList = QString(read_ttm_ary).split("$--TTM");
    //qDebug()<< blockList;
    for (int i = 0; i < blockList.size(); ++i){

        if(blockList.value(i).isEmpty())
            continue;

        QString read_ttm_str= blockList.value(i).prepend("$--TTM");
        QStringList read_ttm_list = read_ttm_str.split(",");

        QString ttm_id = read_ttm_list.value(0);
        QString ttm_id_comp = "TTM";

        /*Filter*/
        if(ttm_id.contains(ttm_id_comp,Qt::CaseSensitive))
        {
            /*Parse the TTM message*/
            parseTtmMsg(read_ttm_list,read_ttm_str);

        }

    }




}


void ReadTtm::parseTtmMsg(QStringList read_ttm_list,QString ttmNmeaMsg)
{

    QString read_ttm_id = read_ttm_list.value(0);
    QString read_ttm_no = read_ttm_list.value(1);
    double distance = read_ttm_list.value(2).toDouble();
    double bearing = read_ttm_list.value(3).toDouble();

    QString unitStr = read_ttm_list.value(10);
    double convFactorDist =  1000;
    double  convFactorSpeed = 0.277778;

    if(unitStr.contains("K")){
        convFactorDist =1000;
        convFactorSpeed = 0.277778;
    }

    if(unitStr.contains("N")){
        convFactorDist =1852;
        convFactorSpeed =0.5144;
    }



    QGeoCoordinate osLoc = QGeoCoordinate(ownship.value("lat").toDouble(),ownship.value("lon").toDouble());
    QGeoCoordinate targetLoc  = osLoc.atDistanceAndAzimuth(distance*convFactorDist,bearing);

    QStringList refinedDataList;
    refinedDataList.append(read_ttm_list.value(0));
    refinedDataList.append(read_ttm_list.value(1));
    refinedDataList.append(QString::number(distance*convFactorDist/1852.0));
    refinedDataList.append(read_ttm_list.value(3));
    refinedDataList.append(read_ttm_list.value(4));
    refinedDataList.append(QString::number(read_ttm_list.value(5).toDouble()*convFactorSpeed));
    refinedDataList.append(read_ttm_list.value(6));
    refinedDataList.append(QString::number(targetLoc.latitude()));
    refinedDataList.append(QString::number(targetLoc.longitude()));
    refinedDataList.append(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()));



    bool newTarget = true;

    for(int i = 0; i<targetList.size();i++)
    {


        int ttmNo = targetList.at(i).value(1).toInt();
        if(read_ttm_no.toInt() ==ttmNo)
        {
            targetList.replace(i,refinedDataList);
            listTtmNmeaMsgs.replace(i,ttmNmeaMsg);
            newTarget = false;
            //qDebug()<<"Target "<<ttmNo<<" Inserted";
            break;
        }

    }
    if(newTarget)
    {
        targetList.append(refinedDataList);
        listTtmNmeaMsgs.append(ttmNmeaMsg);
        //qDebug()<<"Target "<<read_ttm_no.toInt()<<" Appended";
    }

    // draw();

    parseTargets();
    label_test = read_ttm_id;
    //calculate_target_position(target_dist, target_bearing);

    emit sendTtmTargetList(targetList);
}

void ReadTtm::parseTargets(){

    QGeoCoordinate osLoc = QGeoCoordinate(ownship.value("lat").toDouble(),ownship.value("lon").toDouble());
    os_hdg = ownship["hdg"].toDouble();
    for(int i = 0; i<targetList.size();i++)
    {
        double tAngle = os_hdg -targetList.value(i).value(3).toDouble()-90;
        tAngle = qDegreesToRadians(tAngle);
        double tDist =  targetList.value(i).value(2).toDouble();

        QGeoCoordinate tsLoc = osLoc.atDistanceAndAzimuth(tDist,tAngle);

        // qDebug() << QString("TS TTM %1 : %2").arg(i).arg(tsLoc.toString(QGeoCoordinate::DegreesMinutesWithHemisphere));

    }

}

void ReadTtm::draw(){

    scene_radar->clear();
    drawRadarBg();
    os_hdg = ownship["hdg"].toDouble();
    QGraphicsEllipseItem *target;
    for(int i = 0; i<targetList.size();i++)
    {
        /*Radar target*/

        QRectF targetBoundRect;
        targetBoundRect.setCoords(0,0,5,5);
        QPen pen_target(Qt::yellow);
        QBrush brush_target(Qt::yellow);
        target =  scene_radar->addEllipse(targetBoundRect,pen_target,brush_target);


        // int ttmNo = targetList.at(i).value(1).toInt();
        double tAngle = os_hdg -targetList.value(i).value(3).toDouble()-90;
        tAngle = qDegreesToRadians(tAngle);
        double tDist =  targetList.value(i).value(2).toDouble()*radarDialRadius*0.5/range;
        // qDebug()<< "Target "<< ttmNo << "At Dist : " << targetList.value(i).value(2) << " At Angle : " << targetList.value(i).value(3);
        QPointF targetLoc;
        targetLoc.setX(-1*tDist*qCos(tAngle));
        targetLoc.setY(tDist*qSin(tAngle));
        target->setPos(targetLoc);
        //qDebug()<< targetLoc;
    }
}

/*-------------------------------------------------------------------------------------------------Graphics View*/
void ReadTtm::createGraphicView_radar()
{
    /*Create graphics scene*/
    scene_radar=new QGraphicsScene(this);
    ui->graphicsView_radar->setScene(scene_radar);
    ui->graphicsView_radar->setRenderHint(QPainter::Antialiasing);




    drawRadarBg();

}


void ReadTtm::drawRadarBg()
{



    radarDialRadius = (qMin(ui->graphicsView_radar->width(),ui->graphicsView_radar->height()))/2-50;
    QRect radar_rect(0,0,radarDialRadius*2,radarDialRadius*2);
    radar_rect.translate(-radarDialRadius,-radarDialRadius);
    scene_radar->setSceneRect(radar_rect);



    /*Radar bg*/
    QPen pen_radar_dial(Qt::blue);
    QBrush brush_radar_dial(QColor(57,59,179));


    /*Radar Dial*/
    //QRect bounding_rec_radar(radarBoundRect);
    radar_dial = scene_radar->addEllipse(radar_rect,pen_radar_dial,brush_radar_dial);
    radar_dial->setPos(0,0);

    /*Scan line*/
    QPen pen_radar_scanner(Qt::white);
    pen_radar_scanner.setWidth(.5);

    /*tick line*/
    QPen pen_radar_tick(Qt::black);
    pen_radar_tick.setWidth(0.1);

    /*Scanner Line*/
    scanner = scene_radar->addLine(0,0,0,-radarDialRadius,pen_radar_scanner);


    /*Own ship  image*/
    QString imgDirOwn = qApp->applicationDirPath() + "/images/target.png";
    QPixmap target(imgDirOwn);
    item_target = scene_radar->addPixmap(target);
    item_target->setOffset(-0.5*QPointF(29,64));
    //item_target->setRotation(os_hdg);


    /*Text font for display of ROT and Rudder values in the graphics view*/
    QFont serifFont("Segoi UI", 8, QFont::Normal);


    /*Text properties*/
    //  itemHdgText = scene_radar->addText(QString::number(os_hdg).append(" °"),serifFont);

    //    qDebug()<<textWidth;
    // itemHdgText->setPos(itemHdgText->textWidth(),-270);
    //  itemHdgText->setZValue(10);

    double tickPositionAngle,tickLngt;

    QLineF tickLine;
    QPointF tickPt1,tickPt2,textPt;

    for (int j =0;j<72;j++){

        if(j%2==0){
            tickLngt = tickLngtMajor;
        }
        else{
            tickLngt = tickLngtMinor;
        }

        tickPositionAngle =j*5-90-os_hdg;
        tickPositionAngle = qDegreesToRadians(tickPositionAngle);
        tickPt1.setX(radarDialRadius*qCos(tickPositionAngle));
        tickPt1.setY(radarDialRadius*qSin(tickPositionAngle));
        tickPt2.setX((radarDialRadius+tickLngt)*qCos(tickPositionAngle));
        tickPt2.setY((radarDialRadius+tickLngt)*qSin(tickPositionAngle));

        tickLine.setP1(tickPt1);
        tickLine.setP2(tickPt2);

        if(j%2==0){
            itemHdgText = scene_radar->addText(QString("%1°").arg(j*5,3,10,QChar('0')),serifFont);
            itemHdgText->setDefaultTextColor(Qt::black);

            textPt.setX((radarDialRadius+tickLngt+15)*qCos(tickPositionAngle));
            textPt.setY((radarDialRadius+tickLngt+10)*qSin(tickPositionAngle));

            itemHdgText->setPos(textPt);
            itemHdgText->moveBy(-15,-10);

        }
        tickMinor =  scene_radar->addLine(tickLine,pen_radar_tick);
    }

}


void ReadTtm::refreshList()
{
    qDebug() <<"Cleared";
    targetList.clear();
    listTtmNmeaMsgs.clear();
}


void ReadTtm::setOsData(QMap<QString, QString> ownship)
{
    this->ownship =ownship;

    QDateTime simTimeUtc = QDateTime::fromMSecsSinceEpoch(ownship["simTime"].toLongLong()*1000);
    simTimeUtc.setTimeSpec(Qt::LocalTime);
    QDateTime dateTime= simTimeUtc.toUTC();

    ui->lbShipTime->setText(QString("Local Time: %1 | UTC: %2 | Ship Time: %3")
                            .arg(QDateTime::currentDateTime().toString("dd MMM yyyy, hh:mm:ss"))
                            .arg(QDateTime::currentDateTimeUtc().toString("dd MMM yyyy, hh:mm:ss"))
                            .arg(dateTime.toString("dd MMM yyyy, hh:mm:ss")));;



    ui->lbShipName->setText(QString("Ship: %1").arg(ownship["name"]));
    draw();
}

QStringList ReadTtm::getTtmNmeamsg(){
    return listTtmNmeaMsgs;
}

void ReadTtm::on_comboBox_activated(int index)
{
    switch (index) {
    case 0:   // 3NM
        range =3;
        break;
    case 1:  // 6NM
        range =6;
        break;
    case 2:  // 12NM
        range =12;
        break;
    case 3:  // 24 NM
        range =24;
        break;
    case 4:  // 48NM
        range =48;
        break;
    case 5:  // 96 NM
        range =96;
        break;

    default:
        break;
    }

    //qDebug()<< "Range :" << range;
}
