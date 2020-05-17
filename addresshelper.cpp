#include "addresshelper.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QStringList>
#include <klog.h>

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
        // if it's not a ip address, resolve it
        if(addr[0].isDigit() == false)
        {
            QHostInfo hostInfo = QHostInfo::fromName(addr);
            if(hostInfo.addresses().isEmpty())
                return false;
            address = hostInfo.addresses().first();
        }
        else
        {
            address.setAddress(addr);
        }
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
