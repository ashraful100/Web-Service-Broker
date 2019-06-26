#include "shsdatatemplate.h"
#include "ui_shsdatatemplate.h"

ShsDataTemplate::ShsDataTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShsDataTemplate)
{
    ui->setupUi(this);

    //Shs Data Template

    QSettings dataTemplateSettings(qApp->applicationDirPath()+QString("/DataTemplateConfig.ini"),QSettings::IniFormat);
    dataTemplateSettings.beginGroup(QString("Ship%1").arg(1));
    osDataTemplate = dataTemplateSettings.value("osDataTemplate").toString();
    tsDataTemplate = dataTemplateSettings.value("tsDataTemplate").toString();
    dataTemplateSettings.endGroup();

    ui->teOsData->setText(osDataTemplate);
    ui->teTsData->setText(tsDataTemplate);
    //Shs Data Template



}

ShsDataTemplate::~ShsDataTemplate()
{
    delete ui;
}

void ShsDataTemplate::on_pushButtonUpdate_clicked()
{

    osDataTemplate = ui->teOsData->toPlainText();
    tsDataTemplate = ui->teTsData->toPlainText();

    QSettings dataTemplateSettings(qApp->applicationDirPath()+QString("/DataTemplateConfig.ini"),QSettings::IniFormat);
    dataTemplateSettings.beginGroup(QString("Ship%1").arg(1));
    dataTemplateSettings.setValue("osDataTemplate",osDataTemplate);
    dataTemplateSettings.setValue("tsDataTemplate",tsDataTemplate);
    dataTemplateSettings.endGroup();


    emit onShsDataUpdate(osDataTemplate,tsDataTemplate);
}

void ShsDataTemplate::setShsDataTemplate(QString &osDataTemplate,QString &tsDataTemplate){
    this->osDataTemplate = osDataTemplate;
    this->tsDataTemplate = tsDataTemplate;
    ui->teOsData->setText(osDataTemplate);
    ui->teTsData->setText(tsDataTemplate);
}

void ShsDataTemplate::getShsDataTemplate(QString &osDataTemplate,QString &tsDataTemplate)
{
    osDataTemplate =this->osDataTemplate;
    tsDataTemplate = this->tsDataTemplate;
}

QString ShsDataTemplate::getOsDataTemplate() const
{
    return osDataTemplate;
}

void ShsDataTemplate::setOsDataTemplate(const QString &value)
{
    osDataTemplate = value;
}
