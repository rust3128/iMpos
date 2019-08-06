#ifndef VIEWFUELNAMEDIALOG_H
#define VIEWFUELNAMEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class ViewFuelNameDialog;
}

class ViewFuelNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFuelNameDialog(QList<int> *listTerm, QWidget *parent = nullptr);
    ~ViewFuelNameDialog();

private:
    void getConnectionsList();
private:
    Ui::ViewFuelNameDialog *ui;
    QList<int> *m_terminalSList;
    QList<QStringList> m_connectionsList;
};

#endif // VIEWFUELNAMEDIALOG_H
