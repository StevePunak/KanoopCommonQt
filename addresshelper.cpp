#include "addresshelper.h"
#include <QStringList>

bool AddressHelper::tryParseAddressPort(const QString &addressString, QString &address, quint16 &port)
{
    bool result = false;
    QStringList parts = addressString.split(':');
    if(parts.length() == 2)
    {
        address = parts[0];
        port = parts[1].toUShort(&result);
    }
    return result;
}
