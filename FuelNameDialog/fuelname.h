#ifndef FUELNAME_H
#define FUELNAME_H
#include <QString>

class FuelName
{
public:
    FuelName();
    int tankID() const;
    void setTankID(int tankID);

    int fuelID() const;
    void setFuelID(int fuelID);

    QString shortName() const;
    void setShortName(const QString &shortName);

    QString name() const;
    void setName(const QString &name);

private:
    int m_tankID;             //    номер резервуара;
    int m_fuelID;             //    код вида топлива;
    QString m_shortName;      //    краткое наименование топлива;
    QString m_name;           //    полное наименование топлива которое отображается в фискальном чеке.
};

#endif // FUELNAME_H
