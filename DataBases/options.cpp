#include "options.h"
#include "LoggingCategories/loggingcategories.h"

Options::Options(QObject *parent) : QObject(parent)
{
    //используем соедиенеие с базой данных options
    db = QSqlDatabase::database("options");
}

QVariant Options::getOption(int optionID)
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare("SELECT value FROM options WHERE option_id=:optionID");
    q.bindValue(":optionID",optionID);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось получить значение опции" << optionID << ".Причина" << q.lastError().text();
        return QVariant();
    }
    q.next();
    return q.value(0);
}

void Options::setOption(int optionID, QString value)
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare("UPDATE options SET value = :value WHERE option_ID = :optionID");
    q.bindValue(":optionID", optionID);
    q.bindValue(":value", value);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось обновить опцию" << optionID << "Причина" << q.lastError().text();
    }
}


