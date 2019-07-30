#include "addterminaldialog.h"
#include "ui_addterminaldialog.h"
#include "LoggingCategories/loggingcategories.h"
#include "DataBases/options.h"
#include <QKeyEvent>
#include <QPushButton>

AddTerminalDialog::AddTerminalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTerminalDialog)
{
    ui->setupUi(this);

    createModels();
    createUI();
}

AddTerminalDialog::~AddTerminalDialog()
{
    delete ui;
}

void AddTerminalDialog::createModels()
{
    modelTerminals = new QSqlQueryModel(this);
    modelTerminals->setQuery("SELECT DISTINCT t.TERMINAL_ID, TRIM(t.NAME), t.OWNER_ID from terminals t "
                             "LEFT JOIN shifts s ON s.TERMINAL_ID = t.TERMINAL_ID "
                             "WHERE t.TERMINALTYPE=3 and s.SHIFT_ID >1"
                             "ORDER BY t.TERMINAL_ID");
    modelTerminals->setHeaderData(0, Qt::Horizontal,"АЗС");
    modelTerminals->setHeaderData(1, Qt::Horizontal,"Название терминала");
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(modelTerminals);


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

void AddTerminalDialog::createUI()
{
     ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
     ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(false);
    ui->buttonBox->setFocusProxy(ui->tableViewTerminals);
    ui->tableViewTerminals->setModel(proxyModel);
    ui->tableViewTerminals->verticalHeader()->hide();
    ui->tableViewTerminals->hideColumn(2);
    ui->tableViewTerminals->verticalHeader()->setDefaultSectionSize(ui->tableViewTerminals->verticalHeader()->minimumSectionSize());
    ui->tableViewTerminals->resizeColumnsToContents();


    // Выделяем нужную строку

    ui->tableViewTerminals->selectRow(0);

    // Имитируем нажатие кнопки Tab, чтобы выделить строку
    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(this, pe) ;
//    QApplication::sendEvent(this, pe) ;
//    QApplication::sendEvent(this, pe) ;



    ui->comboBoxRegions->setModel(modelRegions);
    ui->comboBoxRegions->setModelColumn(1);
    ui->comboBoxRegions->setCurrentIndex(-1);
}

void AddTerminalDialog::on_comboBoxRegions_activated(int idx)
{
    QString ownerid = modelRegions->data(modelRegions->index(idx,0,QModelIndex())).toString().trimmed();

    proxyModel->setFilterRegExp(QRegExp("^"+ownerid+"$"));
    proxyModel->setFilterKeyColumn(2);

    ui->tableViewTerminals->selectRow(0);

    QKeyEvent* pe = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,Qt::NoModifier, "Tab");
    QApplication::sendEvent(ui->tableViewTerminals, pe);
//    QApplication::sendEvent(this, pe);
//    QApplication::sendEvent(this, pe);

}

void AddTerminalDialog::on_tableViewTerminals_doubleClicked(const QModelIndex &idx)
{
    QModelIndex sourseIdx = proxyModel->mapToSource(idx);
    selectedTerminal = modelTerminals->data(modelTerminals->index(sourseIdx.row(),0,QModelIndex())).toInt();
    this->accept();
}

void AddTerminalDialog::on_buttonBox_accepted()
{
    QModelIndex sourseIdx = ui->tableViewTerminals->selectionModel()->selectedRows().first();
    selectedTerminal = modelTerminals->data(modelTerminals->index(sourseIdx.row(),0,QModelIndex())).toInt();

}
int AddTerminalDialog::getTerminals()
{
    return selectedTerminal;
}
