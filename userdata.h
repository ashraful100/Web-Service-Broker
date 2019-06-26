#ifndef USERDATA_H
#define USERDATA_H

#include <QWidget>

namespace Ui {
class UserData;
}

class UserData : public QWidget
{
    Q_OBJECT

public:
    explicit UserData(QWidget *parent = 0);
    void setData(int reqId, QString userId, QString userName, QString appId);
    ~UserData();

private:
    Ui::UserData *ui;
};

#endif // USERDATA_H
