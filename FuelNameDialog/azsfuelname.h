#ifndef AZSFUELNAME_H
#define AZSFUELNAME_H
#include "fuelname.h"
#include <QList>
#include <QString>

class AzsFuelName
{
public:
    AzsFuelName();
    QList<FuelName> listFuels() const;
    void insertFuelName(int tankID, int fuelID,  QString shortName, QString name); //Добавления наименования топлива
    int terminalID() const;
    void setTerminalID(int terminalID);
private:
    int m_terminalID;           //Номер терминала
    QList<FuelName> m_listFuels;//Список наименований топлива
};

#endif // AZSFUELNAME_H
