#include "usersdialog.h"
#include "ui_usersdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QMessageBox>


UsersDialog::UsersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersDialog)
{
    ui->setupUi(this);

    //Используем соединение options
    db = QSqlDatabase::database("options");

    createUI();
}

UsersDialog::~UsersDialog()
{
    delete ui;
}

void UsersDialog::createUI()
{
    ui->groupBoxUserData->hide();           //Скрываем информацию о пользователе.
    ui->checkBoxIsActive->setChecked(true); //Устанавливаем выбранным
    //Колличество столбцов
    ui->tableWidget->setColumnCount(5);
    //Заолняем TableWidget данными
    createTable();
}

void UsersDialog::createTable()
{
    //заголовки столбцов
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Логин"<<"Пароль"<<""<<"Активность");
    QSqlQuery q = QSqlQuery(db);
    //Получаем список пользователей
    if(!q.exec("select user_id, fio, password, isactive FROM users WHERE user_id > 1 order by user_id"))
        qCritical(logCritical())<< "Не возможно получить список пользователей" << q.lastError().text();
    int i = 0;              //Текущая строка
    //Создаем и заполняем таблицу пользователей
    while(q.next()){
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(q.value(3).toString()));
        if(q.value(3).toBool()){
            ui->tableWidget->setItem(i,4, new QTableWidgetItem(QIcon(":/Image/userActive.png"),"Активен"));
        } else {
            ui->tableWidget->setItem(i,4, new QTableWidgetItem(QIcon(":/Image/userNoActive.png"),"Не Активен"));
        }
        ++i;
    }
    //Скрываем горизонтальный заголовок и 3-й столбец
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->hideColumn(3);
    //Ширина колонки по содержимому
    ui->tableWidget->resizeColumnsToContents();
    //Высота сторок
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);
    ui->pushButtonEdit->setDisabled(true);

}

void UsersDialog::on_pushButtonAdd_clicked()
{
    ui->groupBoxUserData->show();
    ui->tableWidget->setDisabled(true);
    ui->pushButtonAdd->setDisabled(true);
    ui->pushButtonEdit->setDisabled(true);
    isNew = true;
}

void UsersDialog::on_buttonBox_accepted()
{
    //Простая проверка заполнения полей
    if(ui->lineEditLogin->text().size()<3 || ui->lineEditPass->text().size()<1) {
        QMessageBox::warning(this,"Ошибка","Не заполнены обязательные поля.");
        return;
    }
    QSqlQuery q = QSqlQuery(db);
    if(isNew){
        //Новая запись
        q.prepare("INSERT INTO `users`(`fio`,`password`) VALUES (:userName,:passw)");
        q.bindValue(":userName", ui->lineEditLogin->text().trimmed());
        q.bindValue(":passw", ui->lineEditPass->text().trimmed());
        if(q.exec()){
            //Очищаем TableWidget
            ui->tableWidget->clear();
            ui->tableWidget->setRowCount(0);
            //Заолняем TableWidget обновленными данными
            createTable();
        } else {
            qInfo(logInfo()) << "Не удалось добавить опреатора" << q.lastError().text();
        }
    } else {
        //Редактируем запись
        q.prepare("UPDATE users SET fio = :userName, password = :passw, isactive = :isactive WHERE user_id = :userid");
        q.bindValue(":userName", ui->lineEditLogin->text().trimmed());
        q.bindValue(":passw", ui->lineEditPass->text().trimmed());
        q.bindValue(":isactive",(ui->checkBoxIsActive->isChecked()) ? "true" : "false" );
        q.bindValue(":userid", ui->tableWidget->item(currentItem->row(),0)->text());
        if(q.exec()){
            //Очищаем TableWidget
            ui->tableWidget->clear();
            ui->tableWidget->setRowCount(0);
            //Заолняем TableWidget обновленными данными
            createTable();
        } else {
            qInfo(logInfo()) << "Не удалось обновить данные оператора опреатора" << q.lastError().text();
        }
    }
    ui->groupBoxUserData->hide();
    ui->tableWidget->setEnabled(true);
    ui->tableWidget->clearSelection();
    ui->pushButtonAdd->setEnabled(true);
    ui->lineEditPass->clear();
    ui->lineEditLogin->clear();
}

void UsersDialog::on_buttonBox_rejected()
{
    ui->groupBoxUserData->hide();
    ui->tableWidget->setEnabled(true);
    ui->pushButtonAdd->setEnabled(true);
    ui->tableWidget->clearSelection();
    ui->lineEditPass->clear();
    ui->lineEditLogin->clear();
}

void UsersDialog::on_tableWidget_itemSelectionChanged()
{
    //Получаем текущее выделение
    currentItem = ui->tableWidget->currentItem();
    //разрешаем редактирование
    ui->pushButtonEdit->setEnabled(true);
}

void UsersDialog::on_pushButtonEdit_clicked()
{
    ui->groupBoxUserData->show();
    ui->tableWidget->setDisabled(true);
    ui->pushButtonAdd->setDisabled(true);
    ui->pushButtonEdit->setDisabled(true);
    //Заполняем объекты данными о выбранном пользователе
    ui->lineEditLogin->setText(ui->tableWidget->item(currentItem->row(),1)->text());
    ui->lineEditPass->setText(ui->tableWidget->item(currentItem->row(),2)->text());
    ui->checkBoxIsActive->setChecked(QVariant(ui->tableWidget->item(currentItem->row(),3)->text()).toBool());

    isNew = false;

}

void UsersDialog::on_pushButtonClose_clicked()
{
    this->reject();
}

void UsersDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //Вызываем редактирование по двойнгому щелчку мыши
    Q_UNUSED(row);
    Q_UNUSED(column);
    on_pushButtonEdit_clicked();
}
