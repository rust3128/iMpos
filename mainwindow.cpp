#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoggingCategories/loggingcategories.h"

#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>

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
    QSqlDatabase dbopt = QSqlDatabase::database("options");
    QSqlQuery q = QSqlQuery(dbopt);
    q.prepare("select u.fio from options o "
              "inner join users u on o.value = u.user_id "
              "where o.option_id=1020");
    if(!q.exec()){
        qCritical(logCritical()) << "Не далось получить данные пользователя." << q.lastError().text();
    } else {
        q.next();
        QLabel *labelUser = new QLabel(this);
        labelUser->setText("Пользователь: " + q.value(0).toString());
        ui->statusBar->addWidget(labelUser);
    }
}
