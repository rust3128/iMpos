#include "getfuelnameclass.h"
#include "LoggingCategories/loggingcategories.h"

GetFuelNameClass::GetFuelNameClass(QStringList connList, QObject *parent) :
    QObject(parent),
    m_connList(connList)
{
    typedef statusThread st;
    qRegisterMetaType<st>("st");
}

void GetFuelNameClass::getFuelList()
{
    //Устанавливаем текущий статус выполнения
    currentStatus.terminalId=m_connList[0].toInt();
    currentStatus.currentStatus=CONNECT_TO_DATABASE;
    emit signalSendStatus(currentStatus);

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
        currentStatus.currentStatus=ERROR_OPEN_DATABASE;
        //Отправляем статус главному потоку
        emit signalSendStatus(currentStatus);
        //Завершаем выполнение по данной АЗС
        emit finisGetList();
        return;
    }
    //Меняем статус выполнения и отправляем его в главный поток
    currentStatus.currentStatus=SELECT_FUEL_NAME;
    emit signalSendStatus(currentStatus);

    //Создаем и подготавливаем запрос
    QSqlQuery q = QSqlQuery(db);
    q.prepare("select t.TANK_ID, f.FUEL_ID, f.SHORTNAME, f.NAME from FUELS f "
              "LEFT JOIN tanks t ON t.FUEL_ID = f.FUEL_ID "
              "where f.ISACTIVE='T' "
              "order by t.TANK_ID");
    //выполняем запрос
    if(!q.exec()) {
        //Запрос выполнить не удалось
        qCritical(logCritical()) << Q_FUNC_INFO << "Не возможно получить список видов топлива с АЗС." << m_connList[0] << q.lastError().text();
        //Меняем статус выполнения и отправляем его в главный поток
        currentStatus.currentStatus=ERROR_GET_FUEL_NAME;
        emit signalSendStatus(currentStatus);
        emit finisGetList();
        return;
    }
    AzsFuelName _fuelName;
    _fuelName.setTerminalID(m_connList[0].toInt());
    while(q.next()){
        _fuelName.insertFuelName(q.value(0).toInt(),q.value(1).toInt(),q.value(2).toString(),q.value(3).toString());
    }
    currentStatus.currentStatus=FINISHED;
    emit signalSendAzsFuelName(_fuelName);
    emit signalSendStatus(currentStatus);
    emit finisGetList();
}
