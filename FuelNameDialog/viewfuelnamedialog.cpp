#include "viewfuelnamedialog.h"
#include "ui_viewfuelnamedialog.h"
#include "LoggingCategories/loggingcategories.h"

ViewFuelNameDialog::ViewFuelNameDialog(QList<int> *listTerm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFuelNameDialog),
    m_terminalSList(listTerm)
{
    ui->setupUi(this);
    getConnectionsList();
}

ViewFuelNameDialog::~ViewFuelNameDialog()
{
    delete ui;
}

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
        list << q.value(0).toString() << q.value(1).toString() << q.value(2).toString()  << q.value(3).toString();
        m_connectionsList.append(list);
    }
}
