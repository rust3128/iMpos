#ifndef ADDREGIONTERMINALSDIALOG_H
#define ADDREGIONTERMINALSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
namespace Ui {
class AddRegionTerminalsDialog;
}

class AddRegionTerminalsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRegionTerminalsDialog(QWidget *parent = nullptr);
    ~AddRegionTerminalsDialog();

private:
    void createModel();
    void createUI();
private:
    Ui::AddRegionTerminalsDialog *ui;
    QSqlQueryModel *modelRegions;
};

#endif // ADDREGIONTERMINALSDIALOG_H
