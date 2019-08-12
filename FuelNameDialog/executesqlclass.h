#ifndef EXECUTESQLCLASS_H
#define EXECUTESQLCLASS_H
#include "statusthread.h"
#include <QObject>

class ExecuteSqlClass : public QObject
{
    Q_OBJECT
public:
    explicit ExecuteSqlClass(QStringList connList, QStringList sqlList, QObject *parent = nullptr);
    ~ExecuteSqlClass();
signals:
    void finisExecute();                           //Выполнение завершено
    void signalSendStatus(statusThread);           //Текущий статус выполнения
public slots:
    void executeSQL();
private:
    QStringList m_connList;                     //Параменты подключения к базе данный АЗС
    QStringList m_listSQL;                      //Список SQL запросов
    statusThread m_currStatus;                  //Текущий статус выполнения
};

#endif // EXECUTESQLCLASS_H
