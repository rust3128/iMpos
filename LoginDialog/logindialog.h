#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    QSqlRecord getUserData();

private slots:
    void on_comboBoxUser_activated(int idx);

    void on_pushButtonLogin_clicked();

    void on_pushButtonCancel_clicked();

private:
    void createUI();
private:
    Ui::LoginDialog *ui;
    QSqlTableModel *modelUsers;         //Модель таблицы пользователя
    int userID;                         //ID выбранного пользователя
    QString userPass;                   //Пароль выбранного пользователя
    QModelIndex idxModel;               //Индекс модели выбранного пользователя
};

#endif // LOGINDIALOG_H
