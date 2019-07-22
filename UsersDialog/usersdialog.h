#ifndef USERSDIALOG_H
#define USERSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>

namespace Ui {
class UsersDialog;
}

class UsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UsersDialog(QWidget *parent = nullptr);
    ~UsersDialog();

private slots:
    void on_pushButtonAdd_clicked();
    void on_buttonBox_accepted();
    void on_tableWidget_itemSelectionChanged();
    void on_buttonBox_rejected();
    void on_pushButtonEdit_clicked();
    void on_pushButtonClose_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    void createUI();            //Создание первоначального интерефейса
    void createTable();         //Заолнение TableWidget
private:
    Ui::UsersDialog *ui;
    QSqlDatabase db;                    // Объект Базы данных
    QTableWidgetItem *currentItem;      //Текущий оператор.
    bool isNew;                         //Выполняется добавление оператора или правка

};

#endif // USERSDIALOG_H
