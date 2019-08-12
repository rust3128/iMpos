#ifndef STATUSTHREAD_H
#define STATUSTHREAD_H
#include <QObject>

struct statusThread             //Структура для хранения текущего статуса
{
    int terminalId;             //Номер терминала
    int currentStatus;          //Текущий статус операции получения наименования
};

enum statusList {               //Список статусов
    CONNECT_TO_DATABASE,        //Соединение с базой данных
    SELECT_FUEL_NAME,           //Получение наименований
    EXECUTE_SQL,                //Выполнение запроса
    ERROR_OPEN_DATABASE,        //Ошибка отрытия базы данных
    ERROR_GET_FUEL_NAME,        //Ошибка выполнения запроса
    ERROR_EXECUTE_SQL,          //Ошибка выполнения запроса
    FINISHED                    //Удачное завершение запроса
};

//Разрешаем использовать данный тип данных во взаимодействии сигнал-слот
Q_DECLARE_METATYPE(statusThread);

#endif // STATUSTHREAD_H
