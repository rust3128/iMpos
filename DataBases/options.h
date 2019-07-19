#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

class Options : public QObject
{
    Q_OBJECT
public:
    explicit Options(QObject *parent = nullptr);
    QVariant getOption(int optionID);              //Получение значения опции
    void setOption(int optionID, QString value);    //Установка значения опции

signals:

public slots:

private:
    QSqlDatabase db;        //База Данных
};

#endif // OPTIONS_H
