#ifndef AMQPSERVERSETTINGS_H
#define AMQPSERVERSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QDebug>

namespace Ui {
class AmqpServerSettings;
}

struct Server {
    QString name;
    QString url;
};

//!
//! \brief The AmqpServerSettings class sets up the amqp server connections
//! \ingroup shsclient
//!
class AmqpServerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AmqpServerSettings(QWidget *parent = 0);
    ~AmqpServerSettings();

    int getSelectedServer() const;
    void broadcastServerChoice();

signals:

    void signalOnServerChanged(QString serverUrl, QString serverName);

private slots:

    void on_cbSelectedServer_activated(int index);

private:
    Ui::AmqpServerSettings *ui;
    QList<Server> servers;
    int selectedServer;
};

#endif // AMQPSERVERSETTINGS_H
