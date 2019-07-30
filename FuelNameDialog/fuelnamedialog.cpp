#include "fuelnamedialog.h"
#include "ui_fuelnamedialog.h"
#include "addterminaldialog.h"
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
    ui->buttonBox->button(QDialogButtonBox::Apply)->setStyleSheet("background: #00FF80");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet("background: #FA5858");
    ui->buttonBox->button(QDialogButtonBox::Reset)->setStyleSheet("background: #A9BCF5");

//    ui->groupBoxActions->hide();
    ui->pushButtonSelectAll->setEnabled(false);
    ui->pushButtonDeSelectAll->setEnabled(false);
    ui->pushButtonDeleteSelected->setEnabled(false);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMinimumDate(QDate::currentDate());
}

void FuelNameDialog::fillingTerminals()
{
    std::sort(listTerminals.begin(), listTerminals.end());

}

void FuelNameDialog::on_toolButtonSelectTerminal_clicked()
{
    AddTerminalDialog *addTermDlg = new AddTerminalDialog();

    addTermDlg->move(this->parentWidget()->geometry().center().x() - addTermDlg->geometry().center().x(),
                     this->parentWidget()->geometry().center().y() - addTermDlg->geometry().center().y());
    if(addTermDlg->exec() == QDialog::Accepted){
        listTerminals.append(addTermDlg->getTerminals());
        fillingTerminals();


    }

}
