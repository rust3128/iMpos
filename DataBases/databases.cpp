#include "databases.h"
#include "LoggingCategories/loggingcategories.h"
#include "databasesettings.h"
#include "ConnectionSettingDialog/connectionsettingdialog.h"

#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QMessageBox>


DataBases::DataBases(QObject *parent) : QObject(parent)
{

}

bool DataBases::connectOptions()
{
    bool result;

    //Проверяемналичие файла базы данных
    if(QFile(DataBaseSettings::NAME).exists()){
        //Файл существует создаем подключение к базе данных
        qInfo(logInfo()) << "Открываем файл настроек приложения.";

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","options");
        db.setHostName(DataBaseSettings::HOSTNAME);
        db.setDatabaseName(DataBaseSettings::NAME);
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
        db.setHostName(DataBaseSettings::HOSTNAME);
        db.setDatabaseName(DataBaseSettings::NAME);
        if(db.open()){
            QStringList listSQL;  //Список запросов
            QSqlQuery q = QSqlQuery(db);
            //Создаем таблицу OPTIONS и добавлем в нее записи
            listSQL << "CREATE TABLE `options` (`option_id`	INTEGER NOT NULL, `value` TEXT NOT NULL, `comment` TEXT, PRIMARY KEY(`option_id`))";
            listSQL << "INSERT INTO `options`(`option_id`,`value`,`comment`) VALUES (1000, 'false', 'Использовать аутентификацию')";
            listSQL << "INSERT INTO `options`(`option_id`,`value`,`comment`) VALUES (1010, 'false', 'Использовать привязку по региону')";
            listSQL << "INSERT INTO `options`(`option_id`,`value`,`comment`) VALUES (1020, '1', 'Текущий пользователь системы')";
            listSQL << "CREATE TRIGGER use_login AFTER UPDATE "
                       "ON options WHEN (SELECT value FROM options WHERE option_id =1000) = 'false' OR (SELECT value FROM options WHERE option_id =1000) = 0  "
                       "BEGIN "
                       "UPDATE options SET value = 1 WHERE option_id= 1020; "
                       "END";
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

bool DataBases::connectCenralDB()
{
    bool result = true;
    //Указываем имя файла настроек
    QSettings settings(DataBaseSettings::CONFIG_FILE_NAME, QSettings::IniFormat);
    //Создаем подключение по умолчанию к базе данных FireBird
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");

    //Установка параметров подключения из файла настроек
    settings.beginGroup("DATABASE");
    db.setHostName(settings.value("HostName").toString());
    db.setDatabaseName(settings.value("DataBase").toString());
    db.setUserName(settings.value("User").toString());
    db.setPassword(settings.value("Password").toString());
    settings.endGroup();
    //Открываем базу данных, если попытка не удачная вызываем диалог настроек подключения
    if(!db.open()) {
        qCritical(logCritical()) <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << db.lastError().text();
        int rez = QMessageBox::question(nullptr, QObject::tr("Ошибка подключения"),
                              "Не могу установить соединение с базой данных.\nПроверить настройки подключения?",
                              QMessageBox::Yes | QMessageBox::No);
        if(rez == QMessageBox::Yes) {
            ConnectionSettingDialog *connDlg = new ConnectionSettingDialog();
            int result = connDlg->exec();
            if(result == QDialog::Accepted) {
                qInfo(logInfo()) << "Сохранение настроек подключения.";
            }
        }
        //Завершаем работу
        result = false;
    } else {
        qInfo(logInfo()) << "Успешное подключение к центральной базе данных.";
        result = true;
    }

    return result;

}
