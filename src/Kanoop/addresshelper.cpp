#include "Kanoop/addresshelper.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QStringList>
#include <Kanoop/klog.h>
#include <QRegularExpression>

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
            address = hostInfo.addresses().at(0);
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
    return getLocalIP(QStringList());
}

/**
 * @brief AddressHelper::getLocalIP
 * @param allowedInterfaces
 * The list of allowed interface names to search (empty for all)
 * @return The first IPv4 address belonging to an interface in the list of allowed interfaces
 */
QHostAddress AddressHelper::getLocalIP(const QStringList &allowedInterfaces)
{
    QHostAddress result;

    int priority = 0x7fffffff;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces)
    {
        if(interface.flags() & QNetworkInterface::IsUp)
        {
            int index = indexOfRegEx(allowedInterfaces, interface.name());
            if((allowedInterfaces.count() == 0) || (index >= 0 && index < priority))
            {
                priority = index;
                QList<QNetworkAddressEntry> addresses = interface.addressEntries();
                foreach (QNetworkAddressEntry address, addresses)
                {
                    if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    {
                        result = address.ip();
                    }
                }
            }
        }
    }
    return result;
}

/**
 * @brief AddressHelper::indexOfRegEx
 * @param regexList list of regular expressions
 * @param ifName
 * @return index of match or -1 if none found
 *
 * This is the inverse of QStringList::indexOf
 */
int AddressHelper::indexOfRegEx(const QStringList& regexList, const QString &ifName)
{
    for(int i = 0;i < regexList.count();i++)
    {
        QRegularExpression regex(regexList[i]);
        QRegularExpressionMatch match = regex.match(ifName);
        if(match.capturedStart() == 0)
        {
            return i;
        }
    }
    return -1;
}
