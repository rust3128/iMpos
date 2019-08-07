#include "azsfuelname.h"

AzsFuelName::AzsFuelName()
{

}

QList<FuelName> AzsFuelName::listFuels() const
{
    return m_listFuels;
}

void AzsFuelName::insertFuelName(int tankID, int fuelID, QString shortName, QString name)
{
    FuelName fn;
    fn.setTankID(tankID);
    fn.setFuelID(fuelID);
    fn.setShortName(shortName);
    fn.setName(name);
    listFuels().append(fn);
}

int AzsFuelName::terminalID() const
{
    return m_terminalID;
}

void AzsFuelName::setTerminalID(int terminalID)
{
    m_terminalID = terminalID;
}
