#ifndef EDITCONNDIALOG_H
#define EDITCONNDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

namespace Ui {
class EditConnDialog;
}

class EditConnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditConnDialog(int row = -1, QWidget *parent = 0);
    ~EditConnDialog();

signals:
    void signalReady();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::EditConnDialog *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModel();
    void createUI();
    void saveConnections();


};

#endif // EDITCONNDIALOG_H
