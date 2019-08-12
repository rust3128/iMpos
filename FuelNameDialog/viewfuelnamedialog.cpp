#include "viewfuelnamedialog.h"
#include "ui_viewfuelnamedialog.h"
#include "passconv.h"
#include "tasklist.h"
#include "getfuelnameclass.h"
#include "executesqlclass.h"
#include "LoggingCategories/loggingcategories.h"
#include <QThread>
#include <QFile>
#include <QDesktopServices>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

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


ViewFuelNameDialog::ViewFuelNameDialog(QList<int> *listTerm, int currentTask, QStringList ls,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFuelNameDialog),
    m_terminalSList(listTerm),
    m_currentTask(currentTask),
    m_listSQL(ls)
{
    ui->setupUi(this);

    //Описания статосов выполнения запроса
    statusText.insert(CONNECT_TO_DATABASE,"Подключение к базе данных АЗС...");
    statusText.insert(SELECT_FUEL_NAME,"Получение списка видов топлива....");
    statusText.insert(ERROR_OPEN_DATABASE,"Ошибка открытия базы данных АЗС!");
    statusText.insert(EXECUTE_SQL, "Обновление наименования топлива...");
    statusText.insert(ERROR_GET_FUEL_NAME, "Ошибка получения списка наименований топлива!");
    statusText.insert(ERROR_EXECUTE_SQL,"Ошибка выполнения запроса");
    statusText.insert(FINISHED,"Готово!");


    headers <<"Резервуар"<<"Код"<<"Краткое"<<"Полное";
    createUI();
    getConnectionsList();

    if(currentTask == EXECUTE_SQL)
        executeSQL();
    else
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
            case EXECUTE_SQL:
                 ui->tableWidget->item(i,1)->setBackground(QBrush("#D7DF01"));
                 ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/selectfuel.png"));
                break;
            case ERROR_OPEN_DATABASE:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#FE2E2E"));
                ui->tableWidget->item(i,1)->setIcon(QIcon(":/Image/error.png"));
                ui->progressBarGetFuel->setValue(ui->progressBarGetFuel->value()+1);
                colError++;
               break;
            case ERROR_EXECUTE_SQL:
                ui->tableWidget->item(i,1)->setBackground(QBrush("#DF01A5"));
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
    //Проверяем что произошло получение информации от всех АЗС указанных в списке
    //И в зависимости от типа задачи запускаем соответствующую функцию
    if(ui->progressBarGetFuel->value() == m_connectionsList.size()){
        switch (m_currentTask) {
        case VIEW_NAME:
            showFuelName();
            break;
        case XLSX_EXPORT:
            exportXlsx();
            break;
        default:
            break;
        }
    }
}

void ViewFuelNameDialog::slotGetAzsFuelName(AzsFuelName azsFuelname)
{
    //Добавляем полученный список наименований
    m_listFuelName.append(azsFuelname);
}


//Отображение наименований
void ViewFuelNameDialog::showFuelName()
{
    std::sort(m_listFuelName.begin(), m_listFuelName.end(),compare);
    ui->groupBoxProgress->hide();
    ui->tableWidgetView->setColumnCount(4);
    ui->tableWidgetView->setHorizontalHeaderLabels(headers);
    ui->tableWidgetView->verticalHeader()->hide();
    int colAzs = m_listFuelName.size();
    for(int i = 0; i<colAzs; ++i ){
        //Добавляем строку с номером и адресом АЗС
        int row = ui->tableWidgetView->rowCount();
        ui->tableWidgetView->insertRow(row);
        QTableWidgetItem *itemAZS = new QTableWidgetItem(QString::number(m_listFuelName.at(i).terminalID())+" "+m_listFuelName.at(i).azsName());
        itemAZS->setTextAlignment(Qt::AlignHCenter);
        itemAZS->setBackground(QColor("#aaff7f"));
        //Объединяем ячейки
        ui->tableWidgetView->setSpan(row,0,1,4);
        ui->tableWidgetView->setItem(row,0,itemAZS);
        for(int j = 0; j<m_listFuelName.at(i).listFuels().size();++j){
            //Заполняем строки наименованиями
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

void ViewFuelNameDialog::exportXlsx()
{
    std::sort(m_listFuelName.begin(), m_listFuelName.end(),compare);

    Document xlsx;          // Будущий документ
    Format format;          // Формат обычных ячеек
    Format formatMerge;     // Формат объединенной ячейки
    Format formatTitle;     // Формат заголовка

    //Задаем параметры форматирования
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBorderStyle(Format::BorderThin);

    formatMerge.setPatternBackgroundColor(QColor("#aaff7f"));
    formatMerge.setHorizontalAlignment(Format::AlignHCenter);
    formatMerge.setVerticalAlignment(Format::AlignVCenter);
    formatMerge.setBorderStyle(Format::BorderThin);

    formatTitle.setHorizontalAlignment(Format::AlignHCenter);
    formatTitle.setVerticalAlignment(Format::AlignVCenter);
    formatTitle.setFontBold(true);
    formatTitle.setBorderStyle(Format::BorderThin);
    formatTitle.setPatternBackgroundColor(QColor("#A9BCF5"));

    //Необходимо отметить что нумерация строк и столбцов в xlsx документе начинается с 1
    //Устанавливаем ширину столбцов
    xlsx.setColumnWidth(1,10);
    xlsx.setColumnWidth(4,30);

    int columnCount = headers.size();
    int colAzs = m_listFuelName.size();
    //Заполняем заголовки
    for(int i =0; i<columnCount; ++i){
        xlsx.write(1,i+1, headers.at(i),formatTitle);
    }

    //Табличная часть
    int rowX=2;
    for(int i=0; i<colAzs; ++i){
        xlsx.write(rowX,1, QString::number(m_listFuelName.at(i).terminalID())+" "+m_listFuelName.at(i).azsName());
        CellRange cellRange = CellRange(rowX,1,rowX,columnCount);
        xlsx.mergeCells(cellRange, formatMerge);
        for(int j = 0; j<m_listFuelName.at(i).listFuels().size(); ++j) {
            rowX++;
            xlsx.write(rowX,1,m_listFuelName.at(i).listFuels().at(j).tankID(),format);
            xlsx.write(rowX,2,m_listFuelName.at(i).listFuels().at(j).fuelID(),format);
            xlsx.write(rowX,3,m_listFuelName.at(i).listFuels().at(j).shortName(),format);
            xlsx.write(rowX,4,m_listFuelName.at(i).listFuels().at(j).name(),format);
        }
    }
    QFile xlsxFile;
    //Создаем абсолютный путь к файлу
    xlsxFile.setFileName(QApplication::applicationDirPath()+"/"+"FuelName.xlsx");
    xlsx.saveAs(xlsxFile.fileName()); // Сохраняем документ
#ifdef Q_OS_WIN
    QDesktopServices::openUrl(QUrl("file:///"+xlsxFile.fileName(), QUrl::TolerantMode));
#else
    QDesktopServices::openUrl(QUrl("file://"+xlsxFile.fileName(), QUrl::TolerantMode));
#endif
    this->reject();
}

void ViewFuelNameDialog::executeSQL()
{
    //Количетсво обрабатываемх АЗС
    int _azsCount = m_connectionsList.size();
    colError=0;
    ui->progressBarGetFuel->setRange(0, _azsCount);
    ui->progressBarGetFuel->setValue(0);
    ui->progressBarGetFuel->setFormat("Обработано %v из %m. Ошибок "+QString::number(colError));

    for(int i=0; i<_azsCount; i++){
        //Создаем объект класса получения наиметований и потока
        ExecuteSqlClass *execSQL = new ExecuteSqlClass(m_connectionsList.at(i),m_listSQL,this);
        QThread *thread = new QThread();
        //помещаем класс в поток.
        execSQL->moveToThread(thread);
        //Связываем сигналы и слоты
        connect(thread,&QThread::started, execSQL, &ExecuteSqlClass::executeSQL);
        connect(execSQL,&ExecuteSqlClass::signalSendStatus,this,&ViewFuelNameDialog::slotGetStatusThread,Qt::UniqueConnection);
        connect(execSQL,&ExecuteSqlClass::finisExecute, execSQL, &ExecuteSqlClass::deleteLater);
        connect(execSQL,&ExecuteSqlClass::finisExecute, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }

}

void ViewFuelNameDialog::on_buttonBox_rejected()
{
    this->reject();
}
