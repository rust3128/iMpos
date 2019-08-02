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
    void on_buttonBox_rejected();

private:
    void createModels();                    //Создание моделей
    void createUI();                        //первичная настройка интерефейса
private:
    Ui::AddTerminalDialog *ui;
    QSqlQueryModel *modelTerminals;         //Модель терминалов
    QSqlQueryModel *modelRegions;           //Модель регионов
    QSortFilterProxyModel *proxyModel;      //Модель для установки фильтра по региону
    int selectedTerminal;                   //Выбранный терминал
};

#endif // ADDTERMINALDIALOG_H
