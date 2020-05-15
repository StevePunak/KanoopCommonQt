#include "addresshelper.h"
#include <QNetworkInterface>
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

bool AddressHelper::tryParseAddressPort(const QString &addressString, QHostAddress &address, quint16 &port)
{
    QString addr;
    if(tryParseAddressPort(addressString, addr, port))
    {
        address.setAddress(addr);
        return true;
    }
    return false;
}

QHostAddress AddressHelper::getLocalIP()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            return address;
        }
    }
    return QHostAddress();
}
