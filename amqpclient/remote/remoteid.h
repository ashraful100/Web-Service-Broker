#ifndef REMOTEID_H
#define REMOTEID_H

#include <QObject>

class RemoteId : public QObject
{
    Q_OBJECT
public:
    explicit RemoteId(QObject *parent = 0);
    ~RemoteId();

    enum FeedBackSource{
        VIRTUALSHS=1,
        ANS,
        SOC
    };

    enum Controller{
        Control=100,
        Scenario,
        Settings
    };

    enum ControllerID{
        Connect=200,
        Disconnect,
        Reset,
        Play,
        Pause,
        Stop,
        FastForward,
        Server
    };

    enum ScenarioID{
        Voyage=300,
        TP_CONTROL,
        CA_CONTROL,
        HW_CONTROL,
        TS_DOMAIN_RESET

    };

    enum Settings{
        OE_MLS_RADIUS=400
    };

signals:

public slots:
};

#endif // REMOTEID_H
