#ifndef SHSDATATEMPLATE_H
#define SHSDATATEMPLATE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class ShsDataTemplate;
}

class ShsDataTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit ShsDataTemplate(QWidget *parent = 0);
    ~ShsDataTemplate();
    QString getOsDataTemplate() const;
    void setOsDataTemplate(const QString &value);

public slots:

    void setShsDataTemplate(QString &osDataTemplate, QString &tsDataTemplate);
    void getShsDataTemplate(QString &osDataTemplate, QString &tsDataTemplate);
signals:
    void onShsDataUpdate(QString &osDataTemplate,QString &tsDataTemplate);
private slots:
    void on_pushButtonUpdate_clicked();

private:
    Ui::ShsDataTemplate *ui;
    QString osDataTemplate,tsDataTemplate;
};

#endif // SHSDATATEMPLATE_H
