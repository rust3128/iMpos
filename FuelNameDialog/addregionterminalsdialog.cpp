#include "addregionterminalsdialog.h"
#include "ui_addregionterminalsdialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBases/options.h"
#include <QCheckBox>

AddRegionTerminalsDialog::AddRegionTerminalsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRegionTerminalsDialog)

{
    ui->setupUi(this);
    checkedTerminals.clear();
    createModel();
    createUI();
}

AddRegionTerminalsDialog::~AddRegionTerminalsDialog()
{
    delete ui;
}

QList<int> AddRegionTerminalsDialog::getTerminalsLists()
{
    return checkedTerminals;
}

void AddRegionTerminalsDialog::createModel()
{
    modelRegions = new QSqlQueryModel(this);
    Options opt;
    if(opt.getOption(1010).toBool()){
        modelRegions->setQuery("select t.region_id, trim(t.NAME) from TERMINALS t "
                                 "where t.TERMINALTYPE=2 "
                                 "order by t.TERMINAL_ID ");
    } else {
        modelRegions->setQuery("select t.terminal_id, trim(t.NAME) from TERMINALS t "
                                 "where t.TERMINALTYPE=2 "
                                 "order by t.TERMINAL_ID ");
    }
}

void AddRegionTerminalsDialog::createUI()
{
    ui->comboBoxRegions->setModel(modelRegions);
    ui->comboBoxRegions->setModelColumn(1);
    ui->comboBoxRegions->setCurrentIndex(0);


    ui->tableWidgetTerm->setColumnCount(3);
    ui->tableWidgetTerm->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");
    ui->tableWidgetTerm->verticalHeader()->hide();
    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
    ui->comboBoxRegions->activated(0);
    ui->tableWidgetTerm->setFocus();
}

void AddRegionTerminalsDialog::on_comboBoxRegions_activated(int idx)
{
    int ownerid = modelRegions->data(modelRegions->index(idx,0,QModelIndex())).toInt();
    qInfo(logInfo()) << "ownerID" << ownerid;


    ui->tableWidgetTerm->clear();
    ui->tableWidgetTerm->setRowCount(0);
    ui->tableWidgetTerm->setHorizontalHeaderLabels(QStringList() << "" << "АЗС" << "Наименование");

    QSqlQuery q;
    q.prepare("select DISTINCT t.terminal_id, TRIM(t.NAME) from TERMINALS t "
              "LEFT JOIN shifts s ON s.TERMINAL_ID = t.TERMINAL_ID "
              "where t.OWNER_ID=:ownerid and s.SHIFT_ID>1 "
              "order by t.TERMINAL_ID");
    q.bindValue(":ownerid", ownerid);
    if(!q.exec()) qCritical(logCritical()) << "Не возможно получит данные по региону" << ownerid << q.lastError().text();
    int row = 0;
    while(q.next()) {
        ui->tableWidgetTerm->insertRow(row);

        QWidget *checkBoxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
        layoutCheckBox->addWidget(checkBox);
        layoutCheckBox->setAlignment(Qt::AlignCenter);
        layoutCheckBox->setContentsMargins(0,0,0,0);
        checkBox->setChecked(true);

        ui->tableWidgetTerm->setCellWidget(row,0,checkBoxWidget);
        ui->tableWidgetTerm->setItem(row,1, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidgetTerm->setItem(row,2,new QTableWidgetItem(q.value(1).toString()));
        ++row;
    }

    ui->tableWidgetTerm->resizeColumnsToContents();
    ui->tableWidgetTerm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTerm->verticalHeader()->setDefaultSectionSize(ui->tableWidgetTerm->verticalHeader()->minimumSectionSize());
}

void AddRegionTerminalsDialog::on_pushButtonSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(true);
    }
}

void AddRegionTerminalsDialog::on_pushButtonDeSelectAll_clicked()
{
    const int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i = 0; i < rowCount; ++i){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        checkBox->setChecked(false);
    }
}

void AddRegionTerminalsDialog::on_buttonBox_rejected()
{
    this->reject();
}

void AddRegionTerminalsDialog::on_buttonBox_accepted()
{
    checkedTerminals.clear();
    int rowCount = ui->tableWidgetTerm->rowCount();
    for(int i =0; i<rowCount; i++){
        QWidget *item = ui->tableWidgetTerm->cellWidget(i,0);
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(item->layout()->itemAt(0)->widget());
        if(checkBox->isChecked()){
             checkedTerminals.append(ui->tableWidgetTerm->item(i,1)->data(Qt::DisplayRole).toInt());
        }
    }
    this->accept();
}
