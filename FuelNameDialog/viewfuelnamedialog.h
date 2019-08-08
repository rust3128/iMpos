#ifndef VIEWFUELNAMEDIALOG_H
#define VIEWFUELNAMEDIALOG_H
#include "statusthread.h"
#include "azsfuelname.h"
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

private slots:

public slots:
    void slotGetStatusThread(statusThread status);      //Обработка статуса выполнения запроса
    void slotGetAzsFuelName(AzsFuelName azsFuelname);   //Получение списка наименований по терминалу
private:
    void createUI();
    void getConnectionsList();                 //Получения дагнных о подключении к базам данных АЗС
    void fuelNameList();                       //Наименования видов топлива
    void showFuelName();                       //Отображение информации о наименованиях

private:
    Ui::ViewFuelNameDialog *ui;
    QList<int> *m_terminalSList;              //Список терминалов с которыми будем работать
    QList<QStringList> m_connectionsList;     //Cписок данных для подключения к базе данных АЗС
    QList<AzsFuelName> m_listFuelName;        //Список наименований топлива
    QStringList statusText;                   //Список описанияй статуса подключений
    int colError;                             //Количество неудачных попыток
};

#endif // VIEWFUELNAMEDIALOG_H
