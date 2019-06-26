#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class ConnectionStatus;
}

class ConnectionStatus : public QWidget
{
    Q_OBJECT

signals:
    void connectionStatus(bool);

public slots:
    void setTraficSignal(bool _status);

public:
    explicit ConnectionStatus(QWidget *parent = 0);
    ~ConnectionStatus();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::ConnectionStatus *ui;
    bool status=false;
};

#endif // CONNECTIONSTATUS_H
