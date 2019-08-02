#ifndef ADDREGIONTERMINALSDIALOG_H
#define ADDREGIONTERMINALSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
namespace Ui {
class AddRegionTerminalsDialog;
}

class AddRegionTerminalsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRegionTerminalsDialog(QWidget *parent = nullptr);
    ~AddRegionTerminalsDialog();
    QList<int> getTerminalsLists();

private slots:
    void on_comboBoxRegions_activated(int idx);
    void on_pushButtonSelectAll_clicked();
    void on_pushButtonDeSelectAll_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
private:
    void createModel();
    void createUI();
private:
    Ui::AddRegionTerminalsDialog *ui;
    QSqlQueryModel *modelRegions;
    QList<int> checkedTerminals;                //Список выбранных терминалов

};

#endif // ADDREGIONTERMINALSDIALOG_H
