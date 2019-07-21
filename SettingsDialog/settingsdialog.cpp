#include "settingsdialog.h"
#include "ui_settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    createUI();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::createUI()
{
    //Устанавливаем сотояния в зависимости от значений в таблице
    ui->checkBoxLogin->setChecked(opt.getOption(1000).toBool());
    ui->checkBoxRegions->setChecked(opt.getOption(1010).toBool());
}


void SettingsDialog::on_buttonBox_accepted()
{
    //Записываем в таблицу состояние
    opt.setOption(1000,QVariant(ui->checkBoxLogin->isChecked()).toString());
    opt.setOption(1010,QVariant(ui->checkBoxRegions->isChecked()).toString());
}
