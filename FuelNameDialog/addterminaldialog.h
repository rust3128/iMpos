#ifndef ADDTERMINALDIALOG_H
#define ADDTERMINALDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>


namespace Ui {
class AddTerminalDialog;
}

class AddTerminalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTerminalDialog(QWidget *parent = nullptr);
    ~AddTerminalDialog();
    int getTerminals();                     //Возвращаем номер выбранного терминала

private slots:
    void on_comboBoxRegions_activated(int idx);
    void on_tableViewTerminals_doubleClicked(const QModelIndex &index);
    void on_buttonBox_accepted();
private:
    void createModels();
    void createUI();
private:
    Ui::AddTerminalDialog *ui;
    QSqlQueryModel *modelTerminals;
    QSqlQueryModel *modelRegions;
    QSortFilterProxyModel *proxyModel;
    int selectedTerminal;
};

#endif // ADDTERMINALDIALOG_H
