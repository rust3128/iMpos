#ifndef DATABASES_H
#define DATABASES_H

#include <QObject>

class DataBases : public QObject
{
    Q_OBJECT
public:
    explicit DataBases(QObject *parent = nullptr);
    bool connectOptions(); //Подключение к базе данный опций приложения

signals:

public slots:
};



#endif // DATABASES_H
