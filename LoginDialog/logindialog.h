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

private slots:
    void on_comboBoxUser_activated(int idx);
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

private:
    void createUI();        //Первоначальные настройки приложения
    void createModel();     //Создание модели пользователей
private:
    Ui::LoginDialog *ui;
    QSqlTableModel *modelUsers;         //Модель данных пользователей
    int userID;                         //ID выбранного пользователя
    QString userPass;                   //Пароль выбранного пользователя из базы данных
};

#endif // LOGINDIALOG_H
