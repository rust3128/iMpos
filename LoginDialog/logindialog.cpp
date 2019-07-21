#include "logindialog.h"
#include "ui_logindialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBases/options.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    createModel();
    createUI();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::createUI()
{
    //Очишаем информационное сообщение
    ui->labelInfo->clear();

    //Связываем модель и comboBox
    ui->comboBoxUser->setModel(modelUsers);
    //Устанавливаем отображение в comboBox столбца с номером 1
    ui->comboBoxUser->setModelColumn(1);
    //Текущий индекс -1 т.е. ничего не выбрано
    ui->comboBoxUser->setCurrentIndex(-1);

}

void LoginDialog::createModel()
{
    //Используем соединение с базой данный options
    QSqlDatabase db = QSqlDatabase::database("options");
    modelUsers = new QSqlTableModel(this,db);

    modelUsers->setTable("users");
    modelUsers->select();
    modelUsers->setFilter("isactive='true'");

}

void LoginDialog::on_comboBoxUser_activated(int idx)
{
    userPass.clear(); //Очищаем переменную с паролем...

    //Получаем данные из модели
    userID = modelUsers->data(modelUsers->index(idx,0)).toInt();
    userPass = modelUsers->data(modelUsers->index(idx,2)).toString();
}

void LoginDialog::on_buttonBox_rejected()
{
    this->reject();
}

void LoginDialog::on_buttonBox_accepted()
{
    if(ui->comboBoxUser->currentIndex()>=0) {
        if(userPass==ui->lineEditPass->text().trimmed()) {
            qInfo(logInfo()) << QString("Пользователь: %1. Успешный вход в систему.").arg(ui->comboBoxUser->currentText());
            //Запись в таблицу options ID пользователя
            Options opt;
            opt.setOption(1020,QString::number(userID));
            this->accept();
        } else {
            ui->labelInfo->setText("Не верный пароль!");
            qWarning(logInfo()) << QString("Пользователь: %1. Не верный пароль!.").arg(ui->comboBoxUser->currentText());
            ui->lineEditPass->clear();
        }
    }
}
