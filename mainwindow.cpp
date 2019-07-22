#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DataBases/databasesettings.h"
#include "SettingsDialog/settingsdialog.h"
#include "UsersDialog/usersdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QLabel>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI()
{
    //Создаем два QLabels которые поместим в StatusBar
    QLabel *labelUsers = new QLabel();
    QLabel *labelConnections = new QLabel();
    //Получаем данные о текущем пользователе
    QSqlDatabase db = QSqlDatabase::database("options");
    QSqlQuery q = QSqlQuery(db);
    q.exec("SELECT u.user_id, u.fio FROM users u "
           "INNER JOIN options o ON u.user_id = o.value "
           "WHERE o.option_id = 1020");
    q.next();
    labelUsers->setText("Пользователь: "+q.value(1).toString());
    if(q.value(0).toInt() != 1){
        ui->actionUsers->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }

    //Получаем данные о текущем подключении
    QSqlDatabase dbcentr = QSqlDatabase::database();
    labelConnections->setText("База данных:"+dbcentr.hostName()+":"+dbcentr.databaseName());
    //Добавляем виджеты в строку состояния
    //Нормальное сообщение
    ui->statusBar->addWidget(labelUsers);
    //Постоянное сообщение
    ui->statusBar->addPermanentWidget(labelConnections);
}

void MainWindow::on_actionSettings_triggered()
{
    //Создаем объект диалога
    SettingsDialog *settingsDlg = new SettingsDialog(this);
    //позиционируем диалог по центру главного окна
    settingsDlg->move(this->geometry().center().x() - settingsDlg->geometry().center().x(),
                      this->geometry().center().y() - settingsDlg->geometry().center().y());
    //Отображаем диалог
    settingsDlg->exec();
}

void MainWindow::on_actionUsers_triggered()
{
    UsersDialog *usersDlg = new UsersDialog(this);
    usersDlg->move(this->geometry().center().x() - usersDlg->geometry().center().x(),
                   this->geometry().center().y() - usersDlg->geometry().center().y());
    usersDlg->exec();
}
