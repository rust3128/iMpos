#ifndef OPTIONSDATA_H
#define OPTIONSDATA_H

#include <QObject>
#include <QVariant>

class OptionsData : public QObject
{
    Q_OBJECT
public:
    explicit OptionsData(QObject *parent = nullptr);
    QVariant getOption(int optionID);
signals:

public slots:
};

#endif // OPTIONSDATA_H
