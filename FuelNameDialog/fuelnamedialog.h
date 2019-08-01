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

private slots:
    void on_toolButtonSelectTerminal_clicked();
    void on_pushButtonSelectAll_clicked();
    void on_pushButtonDeSelectAll_clicked();
    void on_pushButtonDeleteSelected_clicked();
    void on_toolButtonSelectTermRegions_clicked();
    void on_toolButtonSelectRegion_clicked();
private:
    void createUI();                            //Создание начального интерфейса
    void fillingTerminals(int terminalID);      //Зполнение TableWidget выбранным терминалом
    void fillingTerminals(QList<int> listTerm); //Зполнение TableWidget выбранными терминалами
private:
    Ui::FuelNameDialog *ui;
};

#endif // FUELNAMEDIALOG_H
