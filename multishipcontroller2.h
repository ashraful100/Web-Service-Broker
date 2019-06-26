#ifndef MULTISHIPCONTROLLER_H
#define MULTISHIPCONTROLLER_H

#include <QWidget>

namespace Ui {
class MultiShipController;
}

class MultiShipController : public QWidget
{
    Q_OBJECT

public:
    explicit MultiShipController(QWidget *parent = 0);
    ~MultiShipController();

private:
    Ui::MultiShipController *ui;
};

#endif // MULTISHIPCONTROLLER_H
