#include "viewfuelnamedialog.h"
#include "ui_viewfuelnamedialog.h"
#include "passconv.h"
#include "getfuelnameclass.h"
#include "LoggingCategories/loggingcategories.h"
#include <QThread>

static bool compare(const AzsFuelName& first, const AzsFuelName& second)
{
    if (first.terminalID() < second.terminalID())
    {
        return true;
    }
    else
    {
        return false;
    }
}


ViewFuelNameDialog::ViewFuelNameDialog(QList<int> *listTerm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFuelNameDialog),
    m_terminalSList(listTerm)
{
    ui->setupUi(this);

    //Описания статосов выполнения запроса
    statusText.insert(CONNECT_TO_DATABASE,"Подключение к базе данных АЗС...");
    statusText.insert(SELECT_FUEL_NAME,"Получение списка видов топлива....");
    statusText.insert(ERROR_OPEN_DATABASE,"Ошибка открытия базы данных АЗС!");
    statusText.insert(ERROR_GET_FUEL_NAME, "Ошибка получения списка наименований топлива!");
    statusText.insert(FINISHED,"Готово!");

    createUI();
    getConnectionsList();
    fuelNameList();
}

ViewFuelNameDialog::~ViewFuelNameDialog()
{
    delete ui;
}

void ViewFuelNameDialog::createUI()
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "АЗС" << "Статус");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
    ui->groupBoxView->hide();
}
//Получение параметров подключения к базам данных азс
void ViewFuelNameDialog::getConnectionsList()
{
    QSqlQuery q;
    QString strIN="";
    for(int i=0;i<m_terminalSList->size();++i){
        strIN += QString::number(m_terminalSList->at(i))+",";
    }
    strIN.resize(strIN.size()-1);
    QString strSQL = QString("select c.TERMINAL_ID, c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD from CONNECTIONS c "
                             "where c.TERMINAL_ID IN(%1) and c.CONNECT_ID=2").arg(strIN);
    QStringList list;
    if(!q.exec(strSQL)) qCritical(logCritical()) << "Не удалось получить список терминалов" << q.lastError().text();
    while(q.next()){
        list.clear();
        list << q.value(0).toString() << q.value(1).toString() << q.value(2).toString()  << passConv(q.value(3).toString());
        m_connectionsList.append(list);
    }
}

void ViewFuelNameDialog::fuelNameList()
{
    //Количетсво обрабатываемх АЗС
    int _azsCount = m_connectionsList.size();
    colError=0;
    ui->progressBarGetFuel->setRange(0, _azsCount);
    ui->progressBarGetFuel->setValue(0);
    ui->progressBarGetFuel->setFormat("Обработано %v из %m. Ошибок "+QString::number(colError));


    for(int i=0; i<_azsCount; i++){
        //Создаем объект класса получения наиметований и потока
        GetFuelNameClass *getFuel = new GetFuelNameClass(m_connectionsList.at(i));
        QThread *thread = new QThread();
        //помещаем класс в поток.
        getFuel->moveToThread(thread);
        //Связываем сигналы и слоты
        connect(thread,&QThread::started, getFuel, &GetFuelNameClass::getFuelList);
        connect(getFuel,&GetFuelNameClass::signalSendStatus,this,&ViewFuelNameDialog::slotGetStatusThread,Qt::UniqueConnection);
        connect(getFuel,&GetFuelNameClass::signalSendAzsFuelName,this,&ViewFuelNameDialog::slotGetAzsFuelName,Qt::DirectConnection);
        connect(getFuel, &GetFuelNameClass::finisGetList, getFuel, &GetFuelNameClass::deleteLater);
        connect(getFuel, &GetFuelNameClass::finisGetList, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }
}

void ViewFuelNameDialog::slotGetStatusThread(statusThread status)
{
    if(status.currentStatus == CONNECT_TO_DATABASE){
        //Статус соединения с базой
        //Добавляем строки в TableWidget
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row,0, new QTableWidgetItem(QString::number(status.terminalId)));
        ui->tableWidget->setItem(row,1, new QTableWidgetItem(statusText[status.currentStatus]));
        ui->tableWidget->item(row,0)->setIcon(QIcon(":/Image/azs.png"));
        ui->tableWidget->item(row,1)->setIcon(QIcon(":/Image/database.png"));
        ui->tableWidget->item(row,1)->setBackground(QBrush("#F4FA58"));
        ui->tableWidget->sortByColumn(0,Qt::AscendingOrder);
        return;
    }

    int rowCount = ui->tableWidget->rowCount();
    for (int i=0;i<rowCount;++i) {
        //Находим строку в TableWidget и изменяем ее в соответсвии со статусом
        if(ui->tableWidget->item(i,0)->text().toInt() == status.terminalId){
            ui->tableWidget->item(i,1)->setText(statusText[status.currentStatus]);
            switch (status.currentStatus) {
            case SELECT_FUEL_NAME:
                 ui->tableWidget->item(i,1)->setBackground(QBrush("#D7DF01"));
                 ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/selectfuel.png"));
                break;
            case ERROR_OPEN_DATABASE:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#FE2E2E"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/error.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
                colError++;
               break;
            case ERROR_GET_FUEL_NAME:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#DF01A5"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/error.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
                colError++;
               break;
            case FINISHED:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#BFFF00"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/Accept.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
               break;
            default:
                break;
            }
            break;
        }
    }
    ui->progressBarGetFuel->setFormat("Обработано %v из %m. Ошибок "+QString::number(colError));
    if(ui->progressBarGetFuel->value() == m_connectionsList.size()){
        showFuelName();
    }
}

void ViewFuelNameDialog::slotGetAzsFuelName(AzsFuelName azsFuelname)
{
    //Добавляем полученный список наименований
    m_listFuelName.append(azsFuelname);
}

void ViewFuelNameDialog::showFuelName()
{
    std::sort(m_listFuelName.begin(), m_listFuelName.end(),compare);
    QSqlQuery q;
    ui->groupBoxProgress->hide();
    ui->tableWidgetView->setColumnCount(4);
    ui->tableWidgetView->setHorizontalHeaderLabels(QStringList() <<"Резервуар"<<"Код"<<"Кратко"<<"Полное");
    ui->tableWidgetView->verticalHeader()->hide();
    int colAzs = m_listFuelName.size();
    for(int i = 0; i<colAzs; ++i ){
        int row = ui->tableWidgetView->rowCount();
        ui->tableWidgetView->insertRow(row);
        QTableWidgetItem *itemAZS = new QTableWidgetItem(QString::number(m_listFuelName.at(i).terminalID())+" "+m_listFuelName.at(i).azsName());
        itemAZS->setTextAlignment(Qt::AlignHCenter);
        itemAZS->setBackground(QColor("#aaff7f"));
        ui->tableWidgetView->setSpan(row,0,1,4);
        ui->tableWidgetView->setItem(row,0,itemAZS);
        for(int j = 0; j<m_listFuelName.at(i).listFuels().size();++j){
            int rowName = ui->tableWidgetView->rowCount();
            ui->tableWidgetView->insertRow(rowName);
            ui->tableWidgetView->setItem(rowName,0, new QTableWidgetItem(QString::number(m_listFuelName.at(i).listFuels().at(j).tankID())));
            ui->tableWidgetView->item(rowName,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetView->setItem(rowName,1, new QTableWidgetItem(QString::number(m_listFuelName.at(i).listFuels().at(j).fuelID())));
            ui->tableWidgetView->item(rowName,1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetView->setItem(rowName,2, new QTableWidgetItem(m_listFuelName.at(i).listFuels().at(j).shortName()));
            ui->tableWidgetView->setItem(rowName,3, new QTableWidgetItem(m_listFuelName.at(i).listFuels().at(j).name()));
            ui->tableWidgetView->resizeColumnToContents(3);
        }
    }
    ui->tableWidgetView->verticalHeader()->setDefaultSectionSize(ui->tableWidgetView->verticalHeader()->minimumSectionSize());
    ui->groupBoxView->show();
}
