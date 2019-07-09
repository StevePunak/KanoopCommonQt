#ifndef ADDRESSHELPER_H
#define ADDRESSHELPER_H
#include <QHostAddress>
#include <QString>

class AddressHelper
{
public:
    static bool tryParseAddressPort(const QString& addressString, QString& address, quint16& port);

private:
    AddressHelper();
};

#endif // ADDRESSHELPER_H
