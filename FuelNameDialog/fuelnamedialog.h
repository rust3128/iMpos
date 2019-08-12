#ifndef FUELNAMEDIALOG_H
#define FUELNAMEDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class FuelNameDialog;
}


class FuelNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FuelNameDialog(QWidget *parent = nullptr);
    ~FuelNameDialog();
    void sendListSQL();
signals:
    void signalSendSQL(QStringList);
private slots:
    void on_toolButtonSelectTerminal_clicked();
    void on_pushButtonSelectAll_clicked();
    void on_pushButtonDeSelectAll_clicked();
    void on_pushButtonDeleteSelected_clicked();
    void on_toolButtonSelectTermRegions_clicked();
    void on_toolButtonSelectRegion_clicked();
    void on_groupBoxActions_clicked(bool checked);
    void on_buttonBoxView_rejected();
    void on_buttonBoxView_accepted();
    void on_groupBoxDT_clicked();
    void on_groupBoxVIP_clicked();
    void on_checkBoxDTS_clicked(bool checked);
    void on_checkBoxDTW_clicked(bool checked);
    void on_checkBoxVIPS_clicked(bool checked);
    void on_checkBoxVIPW_clicked(bool checked);
    void on_pushButtonCreateScript_clicked();
    void on_buttonBoxEdit_rejected();
    void on_buttonBoxEdit_accepted();
private:
    void createUI();                            //Создание начального интерфейса
    void fillingTerminals(int terminalID);      //Зполнение TableWidget выбранным терминалом
    void fillingTerminals(QList<int> listTerm); //Зполнение TableWidget выбранными терминалами
private:
    Ui::FuelNameDialog *ui;
    QList<int> listTerminals;                   //Список выбранных терминалов
    QStringList listSQL;
    QString infoMessage;
};

#endif // FUELNAMEDIALOG_H
