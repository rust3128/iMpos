#include "fuelname.h"

FuelName::FuelName()
{

}

int FuelName::tankID() const
{
    return m_tankID;
}

void FuelName::setTankID(int tankID)
{
    m_tankID = tankID;
}

int FuelName::fuelID() const
{
    return m_fuelID;
}

void FuelName::setFuelID(int fuelID)
{
    m_fuelID = fuelID;
}

QString FuelName::shortName() const
{
    return m_shortName;
}

void FuelName::setShortName(const QString &shortName)
{
    m_shortName = shortName;
}

QString FuelName::name() const
{
    return m_name;
}

void FuelName::setName(const QString &name)
{
    m_name = name;
}
