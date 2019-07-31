#include "addregionterminalsdialog.h"
#include "ui_addregionterminalsdialog.h"
#include "DataBases/options.h"

AddRegionTerminalsDialog::AddRegionTerminalsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRegionTerminalsDialog)
{
    ui->setupUi(this);
    createModel();
    createUI();
}

AddRegionTerminalsDialog::~AddRegionTerminalsDialog()
{
    delete ui;
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

    ui->tableWidgetTerminals->setFocus();
}
