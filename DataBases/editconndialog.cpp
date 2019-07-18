#include "editconndialog.h"
#include "ui_editconndialog.h"
#include <QRegExpValidator>

EditConnDialog::EditConnDialog(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditConnDialog)
{
    ui->setupUi(this);

    setupModel();

    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }
    createUI();
}

EditConnDialog::~EditConnDialog()
{
    delete ui;
}

void EditConnDialog::setupModel()
{
    QSqlDatabase dblite = QSqlDatabase::database("options");
    model = new QSqlTableModel(this,dblite);
    model->setTable("connections");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->lineEditName,1);
    mapper->addMapping(ui->lineEditHost,2);
    mapper->addMapping(ui->lineEditDatabase,3);
    mapper->addMapping(ui->lineEditUser,4);
    mapper->addMapping(ui->lineEditPass,5);

    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

}

void EditConnDialog::createUI()
{
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEditHost->setValidator(ipValidator);

}



void EditConnDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        saveConnections();
        this->accept();
        break;
    case QDialogButtonBox::Close:
        this->reject();
        break;
    case QDialogButtonBox::Reset:
        ui->lineEditDatabase->clear();
        ui->lineEditHost->clear();
        ui->lineEditName->clear();
        ui->lineEditPass->clear();
        ui->lineEditUser->clear();
        break;
    default:
        break;
    }
}

void EditConnDialog::saveConnections()
{
    mapper->submit();
    model->submitAll();
    emit signalReady();
}
