#include "databases.h"
#include "LoggingCategories/loggingcategories.h"

#include <QFile>
#include <QSqlQuery>
#include <QSqlError>

#define DATABASE_NAME "iMpos.opt"
#define DATABASE_HOSTNAME "iMpos"

DataBases::DataBases(QObject *parent) : QObject(parent)
{

}

bool DataBases::connectOptions()
{
    bool result;

    //Проверяемналичие файла базы данных
    if(QFile(DATABASE_NAME).exists()){
        //Файл существует создаем подключение к базе данных
        qInfo(logInfo()) << "Открываем файл настроек приложения.";

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName(DATABASE_NAME);
        if(db.open()){
            qInfo(logInfo()) << "Файл настроек открыт успешно";
            result = true;
        } else {
            qCritical(logCritical()) << "Не удалось открыть файл настроек приложения.";
            result = false;
        }
    } else {
        //Файл отсутсвует, создем базу данных
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
        db.setHostName(DATABASE_HOSTNAME);
        db.setDatabaseName(DATABASE_NAME);
        if(db.open()){
            QStringList listSQL;  //Список запросов
            QSqlQuery q = QSqlQuery(db);
            //Создаем таблицу OPTIONS и добавлем в нее записи
            listSQL << "CREATE TABLE `options` (`option_id`	INTEGER NOT NULL, `value` TEXT NOT NULL, `comment` TEXT, PRIMARY KEY(`option_id`))";
            listSQL << "INSERT INTO `options`(`option_id`,`value`,`comment`) VALUES (1000, 'false', 'Использовать аутентификацию')";
            listSQL << "INSERT INTO `options`(`option_id`,`value`,`comment`) VALUES (1010, 'false', 'Использовать привязку по региону')";
            //Создаем таблицу пользователей приложения и добавляем в нее запись
            listSQL << "CREATE TABLE `users` ( `user_id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, `fio` TEXT NOT NULL, `password` TEXT, `isactive` TEXT NOT NULL DEFAULT 'true' )";
            listSQL << "INSERT INTO `users`(`fio`,`password`) VALUES ('Администратор','masterkey')";

            //Выполняем запросы
            for (int i =0;i<listSQL.size();++i) {
                if(!q.exec(listSQL.at(i)))
                    qCritical(logCritical()) << Q_FUNC_INFO << "Не удалось выполнить запрос." << listSQL.at(i) << q.lastError().text();
            }
            qInfo(logInfo()) << "Создан файл настроек приложения.";
            result = true;
        } else {
            qCritical(logCritical()) << "Не удалось создать файл настроек приложения.";
            result = false;
        }

    }

    return result;
}
