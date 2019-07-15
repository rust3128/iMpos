#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "editconndialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    this->setupModel("connections",
                     QStringList() << "ID"
                     << "Наименование"
                     << "Хост"
                     << "База данных"
                     << "Пользователь"
                     << "Пароль");
    this->createUI();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::createUI()
{
    ui->tableViewConnections->setModel(modelConnect);
    ui->tableViewConnections->verticalHeader()->hide();
    ui->tableViewConnections->hideColumn(0);

    ui->tableViewConnections->resizeColumnsToContents();
    ui->tableViewConnections->verticalHeader()->setDefaultSectionSize(ui->tableViewConnections->verticalHeader()->minimumSectionSize());
    connect(ui->tableViewConnections,&QAbstractItemView::doubleClicked,this,&ConnectionDialog::slotEditConnection);

}

void ConnectionDialog::setupModel(const QString &tablename, const QStringList &headers)
{
    QSqlDatabase dblite = QSqlDatabase::database("options");
    modelConnect = new QSqlTableModel(this,dblite);
    modelConnect->setTable(tablename);
    modelConnect->select();

    for(int i=0,j=0; i<modelConnect->columnCount();i++,j++){
        modelConnect->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void ConnectionDialog::on_pushButtonClose_clicked()
{
    this->reject();
}

void ConnectionDialog::on_pushButtonNew_clicked()
{
    EditConnDialog *addConnDlg = new EditConnDialog();
    connect(addConnDlg,&EditConnDialog::signalReady,this,&ConnectionDialog::slotUpdateModel);
    addConnDlg->setWindowTitle("Добавить подключение");
    addConnDlg->exec();
}

void ConnectionDialog::slotUpdateModel()
{
    modelConnect->select();
}

void ConnectionDialog::slotEditConnection(QModelIndex idx)
{
    EditConnDialog *addConnDlg = new EditConnDialog(idx.row());
    connect(addConnDlg,&EditConnDialog::signalReady,this,&ConnectionDialog::slotUpdateModel);
    addConnDlg->setWindowTitle("Редактировать подключение");
    addConnDlg->exec();
}

void ConnectionDialog::on_pushButtonEdit_clicked()
{
    slotEditConnection(ui->tableViewConnections->currentIndex());
}
