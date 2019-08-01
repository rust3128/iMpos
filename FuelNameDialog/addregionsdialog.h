#ifndef ADDREGIONSDIALOG_H
#define ADDREGIONSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class AddRegionsDialog;
}

class AddRegionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRegionsDialog(QWidget *parent = nullptr);
    ~AddRegionsDialog();
    QList<int> getTerminalsLists();

private slots:
    void on_pushButtonSelectAll_clicked();

    void on_pushButtonDeSelectAll_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void createUI();
private:
    Ui::AddRegionsDialog *ui;
    QList<int> checkedTerminals;

};

#endif // ADDREGIONSDIALOG_H
