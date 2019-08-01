#include "addregionsdialog.h"
#include "ui_addregionsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include <QCheckBox>

AddRegionsDialog::AddRegionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRegionsDialog)
{
    ui->setupUi(this);

    createUI();
}

AddRegionsDialog::~AddRegionsDialog()
{
    delete ui;
}

QList<int> AddRegionsDialog::getTerminalsLists()
{
    return checkedTerminals;
}

void AddRegionsDialog::createUI()
{
    ui->tableWidgetTerm->setColumnCount(3);
    ui->tableWidgetTerm->setHorizontalHeaderLabels(QStringList() << "" << "Код" << "Регион");
    ui->tableWidgetTerm->verticalHeader()->hide();

    QSqlQuery q;
    q.prepare("select t.TERMINAL_ID, trim(t.NAME) from TERMINALS t "
               "where t.TERMINALTYPE=2 "
               "order by t.TERMINAL_ID");
    if(!q.exec()) qCritical(logCritical()) << "Не удалось получить список регионов" << q.lastError().text();
    int row = 0;
    while(q.next()) {
        ui->tableWidgetTerm->insertRow(row);

        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0,0,0,0);
        checkBox->setChecked(false);

        ui->tableWidgetTerm->setCellWidget(row,0,checkBoxWidget);
        ui->tableWidgetTerm->setItem(row,1, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidgetTerm->setItem(row,2,new QTableWidgetItem(q.value(1).toString()));
        ++row;
    }
    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->hideColumn(1);
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTerm->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerm->verticalHeader()->minimumSectionSize());

    ui->tableWidgetTerm->setFocus();

}

void AddRegionsDialog::on_pushButtonSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(true);
    }
}

void AddRegionsDialog::on_pushButtonDeSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(false);
    }
}

void AddRegionsDialog::on_buttonBox_accepted()
{
    checkedTerminals.clear();

    QString strIN="";
    int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i =0; i<rowCount; i++){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        if(checkBox->isChecked()){

            strIN += ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toString()+",";

        }
    }
    strIN.resize(strIN.size()-1);
    QString strSQL = QString("select t.TERMINAL_ID from TERMINALS t  where t.OWNER_ID in (%1) order by t.TERMINAL_ID").arg(strIN);
    QSqlQuery q;
    if(!q.exec(strSQL)) qCritical(logCritical()) << "Не удалось получить список терминалов" << q.lastError().text();
    while(q.next()){
        checkedTerminals << q.value(0).toInt();
    }
    this->accept();
}

void AddRegionsDialog::on_buttonBox_rejected()
{
    this->reject();
}
