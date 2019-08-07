#ifndef GETFUELNAMECLASS_H
#define GETFUELNAMECLASS_H
#include "statusthread.h"
#include "azsfuelname.h"
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>

class GetFuelNameClass : public QObject
{
    Q_OBJECT
public:
    explicit GetFuelNameClass(QStringList connList, QObject *parent = nullptr);

signals:
    void finisGetList();                           //Получение наименований завершено
    void signalSendStatus(statusThread);        //Текущий статус получеия наименований
    void signalSendAzsFuelName(AzsFuelName);    //Отправка наименований видов топлива в основной поток
public slots:
    void getFuelList();                         //Получение наименований топлива
private:
    QStringList m_connList;                     //Параменты подключения к базе данный АЗС
    statusThread currentStatus;                 //Текущий статус выполнения
};

#endif // GETFUELNAMECLASS_H
