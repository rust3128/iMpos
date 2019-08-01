#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "addterminaldialog.h"
#include "addregionterminalsdialog.h"
#include "addregionsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QGroupBox>
#include <QDate>

FuelNameDialog::FuelNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FuelNameDialog)
{
    ui->setupUi(this);

    createUI();

}

FuelNameDialog::~FuelNameDialog()
{
    delete ui;
}

void FuelNameDialog::createUI()
{
    //Расскрашиваем кнопки QDialogButtonBox
    ui->buttonBox->button(QDialogButtonBox::Apply)->setStyleSheet("background: #00FF80");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet("background: #FA5858");
    ui->buttonBox->button(QDialogButtonBox::Reset)->setStyleSheet("background: #A9BCF5");

    //Деактивируем кнопки управления пока нет добавленных терминалов
    ui->pushButtonSelectAll->setEnabled(false);
    ui->pushButtonDeSelectAll->setEnabled(false);
    ui->pushButtonDeleteSelected->setEnabled(false);

    //Устанавливаем текущую дату и запрещаем выделять дату меньше текущей
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMinimumDate(QDate::currentDate());

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

    for (int i = 0; i<row; ++i) {
        if( terminalID == ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toInt())
            return;
    }
    ui->tableWidgetTerm->insertRow(row);
    QWidget *checkBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
    layoutCheckBox->addWidget(checkBox);
    layoutCheckBox->setAlignment(Qt::AlignCenter);
    layoutCheckBox->setContentsMargins(0,0,0,0);
    checkBox->setChecked(true);
    ui->tableWidgetTerm->setCellWidget(row,0,checkBoxWidget);
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

void FuelNameDialog::on_toolButtonSelectTerminal_clicked()
{
    AddTerminalDialog *addTermDlg = new AddTerminalDialog(this);

    addTermDlg->move(this->parentWidget()->geometry().center().x() - addTermDlg->geometry().center().x(),
                     this->parentWidget()->geometry().center().y() - addTermDlg->geometry().center().y());
    if(addTermDlg->exec() == QDialog::Accepted){
//        listTerminals.append(addTermDlg->getTerminals());
        fillingTerminals(addTermDlg->getTerminals());


    }

}

void FuelNameDialog::on_pushButtonSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(true);
    }
}

void FuelNameDialog::on_pushButtonDeSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(false);
    }
}

void FuelNameDialog::on_pushButtonDeleteSelected_clicked()
{
    int rowCount = ui->tableWidgetTerm->rowCount();
    int uncheckCount=0;
    for(int i =0; i<rowCount; i++){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        if(!checkBox->isChecked()){
            uncheckCount++;
        }
    }
    int currentRow = 0;
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

void FuelNameDialog::on_toolButtonSelectTermRegions_clicked()
{
    AddRegionTerminalsDialog *addRegTermDlg = new AddRegionTerminalsDialog(this);

    addRegTermDlg->move(this->parentWidget()->geometry().center().x() - addRegTermDlg->geometry().center().x(),
                        this->parentWidget()->geometry().center().y() - addRegTermDlg->geometry().center().y());
    if(addRegTermDlg->exec() == QDialog::Accepted){
        fillingTerminals(addRegTermDlg->getTerminalsLists());
    }

}

void FuelNameDialog::on_toolButtonSelectRegion_clicked()
{
    AddRegionsDialog *addRegDlg = new AddRegionsDialog(this);

    addRegDlg->move(this->parentWidget()->geometry().center().x() - addRegDlg->geometry().center().x(),
                        this->parentWidget()->geometry().center().y() - addRegDlg->geometry().center().y());
    if(addRegDlg->exec() == QDialog::Accepted){
        fillingTerminals(addRegDlg->getTerminalsLists());
    }
}
