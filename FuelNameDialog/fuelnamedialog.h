#ifndef FUELNAMEDIALOG_H
#define FUELNAMEDIALOG_H

#include <QDialog>
#include <QGroupBox>

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

private:
    void createUI();
    void fillingTerminals();
private:
    Ui::FuelNameDialog *ui;
    QList<int> listTerminals;              //Список терминалов
};

#endif // FUELNAMEDIALOG_H
