#ifndef STMTESTER_H
#define STMTESTER_H

#include <QWidget>

namespace Ui {
class STMTester;
}

class STMTester : public QWidget
{
    Q_OBJECT

public:
    explicit STMTester(QWidget *parent = 0);
    ~STMTester();
signals:

    void signalPost(int mmsi);
    void signalSub(int mmsi);
    void signalGet(int mmsi);
    void signalGetMsg(int mmsi);

private slots:
    void on_pbSub_clicked();

    void on_pbPost_clicked();

    void on_pbGet_clicked();

    void on_pbGetMsg_clicked();

private:
    Ui::STMTester *ui;
};

#endif // STMTESTER_H
