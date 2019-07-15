#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

private slots:
    void on_pushButtonClose_clicked();
    void on_pushButtonNew_clicked();
    void slotUpdateModel();
    void slotEditConnection(QModelIndex idx);
    void on_pushButtonEdit_clicked();

private:
    void createUI();
    void setupModel(const QString &tablename, const QStringList &headers);              //Создание модели
private:
    Ui::ConnectionDialog *ui;
    QSqlTableModel *modelConnect;

};

#endif // CONNECTIONDIALOG_H
