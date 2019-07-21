#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "DataBases/options.h"
#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    Options opt;                        //чтение запись таблицы options
private:
    void createUI();                    //Настройка первоначального интерфейса
};

#endif // SETTINGSDIALOG_H
