#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "addterminaldialog.h"
#include "addregionterminalsdialog.h"
#include "addregionsdialog.h"
#include "viewfuelnamedialog.h"
#include "tasklist.h"
#include "DataBases/options.h"
#include "LoggingCategories/loggingcategories.h"
#include "SQLHighlighter/SQLHighlighter.h"
#include <QGroupBox>
#include <QDate>
#include <QMessageBox>

FuelNameDialog::FuelNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuelNameDialog)
{
    ui->setupUi(this);
    listSQL.clear();
    createUI();

}

FuelNameDialog::~FuelNameDialog()
{
    delete ui;
}

void FuelNameDialog::createUI()
{
    //Раскрашиваем кнопки QDialogButtonBox
    ui->buttonBoxView->button(QDialogButtonBox::Ok)->setStyleSheet("background: #00FF80");
    ui->buttonBoxView->button(QDialogButtonBox::Cancel)->setStyleSheet("background: #FA5858");
    ui->buttonBoxEdit->button(QDialogButtonBox::Ok)->setStyleSheet("background: #00FF80");
    ui->buttonBoxEdit->button(QDialogButtonBox::Cancel)->setStyleSheet("background: #FA5858");


    //Деактивируем кнопки управления пока нет добавленных терминалов
    ui->pushButtonSelectAll->setEnabled(false);
    ui->pushButtonDeSelectAll->setEnabled(false);
    ui->pushButtonDeleteSelected->setEnabled(false);

    //Устанавливаем текущую дату и запрещаем выделять дату меньше текущей
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    ui->textEditSQL->hide();

    //Настраиваем внешний вид TableWidget
    ui->tableWidgetTerm->setColumnCount(3);
    ui->tableWidgetTerm->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");
    ui->tableWidgetTerm->verticalHeader()->hide();
    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
}
//Добавление информации о выбранном терминале
void FuelNameDialog::fillingTerminals(int terminalID)
{
    ui->pushButtonSelectAll->setEnabled(true);
    ui->pushButtonDeSelectAll->setEnabled(true);
    ui->pushButtonDeleteSelected->setEnabled(true);

    int row = ui->tableWidgetTerm->rowCount();
    //Если терминал есть в списке его не добавляем.
    for (int i = 0; i<row; ++i) {
        if( terminalID == ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toInt())
            return;
    }

    ui->tableWidgetTerm->insertRow(row);
    //В столбце 0 добавляем CheckBox
    QWidget *checkBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
    layoutCheckBox->addWidget(checkBox);
    layoutCheckBox->setAlignment(Qt::AlignCenter);
    layoutCheckBox->setContentsMargins(0,0,0,0);
    checkBox->setChecked(true);
    ui->tableWidgetTerm->setCellWidget(row,0,checkBoxWidget);
    //Получаем данные о терминале из базы данных и добавляем их в TableWidget
    QSqlQuery q;
    q.prepare("select TRIM(t.NAME) from TERMINALS t where t.TERMINAL_ID=:terminalID");
    q.bindValue(":terminalID", terminalID);
    if(!q.exec()) qCritical(logCritical()) << "Не возможно получит данные по терминалу" << terminalID << q.lastError().text();
    q.next();
    ui->tableWidgetTerm->setItem(row,1, new QTableWidgetItem(QString::number(terminalID)));
    ui->tableWidgetTerm->setItem(row,2,new QTableWidgetItem(q.value(0).toString()));
    ui->tableWidgetTerm->sortByColumn(1,Qt::AscendingOrder);

    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTerm->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerm->verticalHeader()->minimumSectionSize());

}
//Добавление информации о выбранном списке терминалов
void FuelNameDialog::fillingTerminals(QList<int> listTerm)
{
    foreach(int terminalID, listTerm){
        fillingTerminals(terminalID);
    }
}
//Вызываем диалог добавления одного терминала
void FuelNameDialog::on_toolButtonSelectTerminal_clicked()
{
    AddTerminalDialog *addTermDlg = new AddTerminalDialog(this);

    addTermDlg->move(this->parentWidget()->geometry().center().x() - addTermDlg->geometry().center().x(),
                     this->parentWidget()->geometry().center().y() - addTermDlg->geometry().center().y());
    if(addTermDlg->exec() == QDialog::Accepted){
        fillingTerminals(addTermDlg->getTerminals());
    }

}
//Вызываем диалог добавления терминалов региона
void FuelNameDialog::on_toolButtonSelectTermRegions_clicked()
{
    AddRegionTerminalsDialog *addRegTermDlg = new AddRegionTerminalsDialog(this);

    addRegTermDlg->move(this->parentWidget()->geometry().center().x() - addRegTermDlg->geometry().center().x(),
                        this->parentWidget()->geometry().center().y() - addRegTermDlg->geometry().center().y());
    if(addRegTermDlg->exec() == QDialog::Accepted){
        fillingTerminals(addRegTermDlg->getTerminalsLists());
    }

}
//Вызываем диалог добавления региона
void FuelNameDialog::on_toolButtonSelectRegion_clicked()
{
    AddRegionsDialog *addRegDlg = new AddRegionsDialog(this);

    addRegDlg->move(this->parentWidget()->geometry().center().x() - addRegDlg->geometry().center().x(),
                        this->parentWidget()->geometry().center().y() - addRegDlg->geometry().center().y());
    if(addRegDlg->exec() == QDialog::Accepted){
        fillingTerminals(addRegDlg->getTerminalsLists());
    }
}
//Устанавливаем выбор на всех CheckBox в таблице
void FuelNameDialog::on_pushButtonSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(true);
    }
}
//Удаляем выбор на на всех CheckBox в таблице
void FuelNameDialog::on_pushButtonDeSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(false);
    }
}
//Удаление выбранных терминалов из TableWidget
void FuelNameDialog::on_pushButtonDeleteSelected_clicked()
{
    int rowCount = ui->tableWidgetTerm->rowCount();
    int uncheckCount=0;                 //Колличество не выбранных строк
    for(int i =0; i<rowCount; i++){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        if(!checkBox->isChecked()){
            uncheckCount++;
        }
    }
    int currentRow = 0;
    //Перебираем строки таблицы и если CheckBox выбран удалянм строку
    //если не выбран переходим на следующую строку
    while(rowCount != uncheckCount){
        QWidget *item = ui->tableWidgetTerm->cellWidget(currentRow,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        if(checkBox->isChecked()){
            ui->tableWidgetTerm->removeRow(currentRow);
            rowCount = ui->tableWidgetTerm->rowCount();
        } else {
            currentRow++;
        }
    }
    if(rowCount == 0){
        ui->pushButtonSelectAll->setEnabled(false);
        ui->pushButtonDeSelectAll->setEnabled(false);
        ui->pushButtonDeleteSelected->setEnabled(false);
    }
}



void FuelNameDialog::on_groupBoxActions_clicked(bool checked)
{
    if(checked){
        //Очищаем список терминалов передаваемых для дальнейшей обработки
        listTerminals.clear();
        //Добавляем отмеченные терминалы в список
        int rowCount = ui->tableWidgetTerm->rowCount();
        for(int i=0; i<rowCount; ++i){
            QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
            QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
            if(checkBox->isChecked()){
               listTerminals.append(ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toInt());
            }
        }
        //Проверяем что список не пустой
        if(listTerminals.size()==0){
            QMessageBox::warning(this, "Ошибка ввода","Нет выбранных терминалов");
            ui->groupBoxActions->setChecked(false);
            return;
        }
    }
    ui->groupBoxFuel->setEnabled(!checked);
}

void FuelNameDialog::on_buttonBoxView_rejected()
{
    ui->groupBoxFuel->setEnabled(true);
    ui->groupBoxActions->setChecked(false);
}

void FuelNameDialog::on_buttonBoxView_accepted()
{
    //Определяем дальнейший алгоритм работы в зависимости от выбранного checkBox
    int currentTask = (ui->radioButtonReport->isChecked()) ? VIEW_NAME : XLSX_EXPORT;
    //Диалог для отображения результатов и прогресса получения данных с АЗС
    ViewFuelNameDialog *viewFnDlg = new ViewFuelNameDialog(&listTerminals,currentTask,listSQL, this);
    viewFnDlg->exec();
}



void FuelNameDialog::on_groupBoxDT_clicked()
{
    ui->checkBoxDTS->setChecked(false);
    ui->checkBoxDTW->setChecked(false);

}

void FuelNameDialog::on_groupBoxVIP_clicked()
{
    ui->checkBoxVIPS->setChecked(false);
    ui->checkBoxVIPW->setChecked(false);
}

void FuelNameDialog::on_checkBoxDTS_clicked(bool checked)
{
    ui->checkBoxDTW->setChecked(!checked);
}

void FuelNameDialog::on_checkBoxDTW_clicked(bool checked)
{
    ui->checkBoxDTS->setChecked(!checked);
}

void FuelNameDialog::on_checkBoxVIPS_clicked(bool checked)
{
    ui->checkBoxVIPW->setChecked(!checked);
}

void FuelNameDialog::on_checkBoxVIPW_clicked(bool checked)
{
    ui->checkBoxVIPS->setChecked(!checked);
}


void FuelNameDialog::on_pushButtonCreateScript_clicked()
{
    if(ui->checkBoxDTS->isChecked() || ui->checkBoxDTW->isChecked() || ui->checkBoxVIPS->isChecked() || ui->checkBoxVIPW->isChecked())
        ui->textEditSQL->show();
    else {
        QMessageBox::critical(this, "Ошибка", "Не указан ни один вид топлива!");
        return;
    }
    listSQL.clear();
    QString dtName="";

    if(ui->checkBoxDTS->isChecked()) {
        dtName = ui->checkBoxDTS->text();
    } else {
        if(ui->checkBoxDTW->isChecked()){
            dtName=ui->checkBoxDTW->text();
        }
    }

    QString VPName="";

    if(ui->checkBoxVIPS->isChecked()) {
        VPName = ui->checkBoxVIPS->text();
    } else {
        if(ui->checkBoxVIPW->isChecked()){
            VPName=ui->checkBoxVIPW->text();
        }
    }


    infoMessage = ui->dateEdit->date().toString("dd.MM.yyyy") + " будут устанвлены следующие наименования\n" + dtName + "\n" + VPName+".";


    if(dtName.size()>0)
        listSQL << "UPDATE FUELS SET NAME = '"+dtName+"' WHERE FUEL_ID = 7;";
    if(VPName.size()>0)
        listSQL << "UPDATE FUELS SET NAME = '"+VPName+"' WHERE FUEL_ID = 8;";

    listSQL << "UPDATE OR INSERT INTO MIGRATEOPTIONS (MIGRATEOPTION_ID, SVALUE, VTYPE) VALUES (3400, '"+ui->dateEdit->date().toString("yyyyMMdd")+"', 'D') MATCHING (MIGRATEOPTION_ID)";
    listSQL << "UPDATE OR INSERT INTO MIGRATEOPTIONS (MIGRATEOPTION_ID, SVALUE, VTYPE) VALUES (3410, '6', 'I') MATCHING (MIGRATEOPTION_ID)";
    listSQL << "commit;";
    ui->textEditSQL->clear();
    new SQLHighlighter(ui->textEditSQL->document());
    ui->textEditSQL->append(listSQL.join("\n"));
}

void FuelNameDialog::on_buttonBoxEdit_rejected()
{
    ui->groupBoxDT->setChecked(false);
    ui->groupBoxVIP->setChecked(false);
    ui->groupBoxActions->setChecked(false);
    ui->checkBoxVIPS->setChecked(false);
    ui->checkBoxVIPW->setChecked(false);
    ui->checkBoxDTS->setChecked(false);
    ui->checkBoxDTW->setChecked(false);
    ui->textEditSQL->clear();
    ui->groupBoxFuel->setEnabled(true);
}

void FuelNameDialog::on_buttonBoxEdit_accepted()
{
    if(listSQL.size() == 0) return;
    int ret = QMessageBox::question(this, "Подтвердите действие",
                                   "На выбранных АЗС будут произведены следующие действия:\n"+infoMessage,
                                   QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == QMessageBox::Ok ){
        //Диалог для отображения результатов и прогресса получения данных с АЗС
        ViewFuelNameDialog *viewFnDlg = new ViewFuelNameDialog(&listTerminals,UPDATE_FUEL_NAME,listSQL, this);
        QSqlDatabase db = QSqlDatabase::database("options");
        QSqlQuery q = QSqlQuery(db);
        Options opt;
        QString terminals;
        foreach(int term, listTerminals){
            terminals += QString::number(term)+",";
        }
        terminals.resize(terminals.size()-1);
        q.prepare("INSERT INTO logs (`date`, `userID`, `logtypeID`,`info`) VALUES (datetime('now','localtime'), :userID, 2, :info)");
        q.bindValue(":userID", opt.getOption(1020).toInt());
        q.bindValue(":info", terminals);
        if(!q.exec()) qCritical(logCritical()) << "Не удалось записать данные об изменении наменований" << q.lastError().text();
        viewFnDlg->exec();

    }
}
