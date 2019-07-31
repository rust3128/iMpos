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

private:
    void createUI();
    void fillingTerminals(int terminalID);
    void fillingTerminals(QList<int> listTerm);
private:
    Ui::FuelNameDialog *ui;
    QList<int> listTerminals;              //Список терминалов
};

#endif // FUELNAMEDIALOG_H
