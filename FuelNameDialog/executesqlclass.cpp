#include "executesqlclass.h"
#include "LoggingCategories/loggingcategories.h"
#include <QSqlQuery>
#include <QSqlError>

ExecuteSqlClass::ExecuteSqlClass(QStringList connList, QStringList sqlList, QObject *parent) :
    QObject(parent),
    m_connList(connList),
    m_listSQL(sqlList)
{
    typedef statusThread st;
    qRegisterMetaType<st>("st");
}

ExecuteSqlClass::~ExecuteSqlClass()
{
    QSqlDatabase::removeDatabase(m_connList[0]);
}

void ExecuteSqlClass::executeSQL()
{
    //Устанавливаем текущий статус выполнения
    m_currStatus.terminalId=m_connList[0].toInt();
    m_currStatus.currentStatus=CONNECT_TO_DATABASE;
    emit signalSendStatus(m_currStatus);
    //Cоздаем подключение к базе данных АЗС
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE", m_connList[0]);

    db.setHostName(m_connList[1]);
    db.setDatabaseName(m_connList[2]);
    db.setUserName("SYSDBA");
    db.setPassword(m_connList[3]);

    //Подключаемся к базе данных АЗС
    if(!db.open()){
        //Не удалось подключится
        qCritical(logCritical()) << Q_FUNC_INFO << "Невозможно подключится к базе данных АСЗ" << m_connList[0] << db.lastError().text();
        //Меняем статус выполнения
        m_currStatus.currentStatus=ERROR_OPEN_DATABASE;
        //Отправляем статус главному потоку
        emit signalSendStatus(m_currStatus);
        //Завершаем выполнение по данной АЗС
        emit finisExecute();
        return;
    }
    //Меняем статус выполнения и отправляем его в главный поток
    m_currStatus.currentStatus=SELECT_FUEL_NAME;
    emit signalSendStatus(m_currStatus);
    //Выполняем запросы
    QSqlQuery q = QSqlQuery(db);
    foreach(const QString &strSQL, m_listSQL ) {
        if(!q.exec(strSQL))
            qCritical(logCritical()) << Q_FUNC_INFO << "Не удалось выполнить запрос." << strSQL << "На базе АЗС" << m_connList[0] << q.lastError().text();
        //Меняем статус выполнения и отправляем его в главный поток
        m_currStatus.currentStatus=ERROR_EXECUTE_SQL;
        emit signalSendStatus(m_currStatus);
        emit finisExecute();
        return;
    }
    m_currStatus.currentStatus=FINISHED;
    emit signalSendStatus(m_currStatus);
    emit finisExecute();
}
