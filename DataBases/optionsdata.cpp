#include "optionsdata.h"
#include "LoggingCategories/loggingcategories.h"
#include <QSqlQuery>
#include <QSqlError>

OptionsData::OptionsData(QObject *parent) : QObject(parent)
{

}

QVariant OptionsData::getOption(int optionID)
{
    QSqlDatabase db = QSqlDatabase::database("options");
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

void OptionsData::setOptions(int optionID, QString value)
{
    QSqlDatabase db = QSqlDatabase::database("options");
    QSqlQuery q = QSqlQuery(db);
    q.prepare("UPDATE options set value = :value WHERE option_id=:optionID");
    q.bindValue(":optionID",optionID);
    q.bindValue(":value", value);
    if(!q.exec()) {
        qCritical(logCritical()) << "Не удалось установить значение опции" << optionID << ".Причина" << q.lastError().text();

    }
}
