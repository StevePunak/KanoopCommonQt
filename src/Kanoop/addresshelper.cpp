#include "Kanoop/addresshelper.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QStringList>
#include <Kanoop/log.h>
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

QHostAddress AddressHelper::resolveIPv4Address(const QString& hostName)
{
    QHostAddress result(hostName);
    if(result.protocol() == QAbstractSocket::IPv4Protocol) {
        return result;
    }
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    for(const QHostAddress& address : hostInfo.addresses()) {
        if(address.protocol() == QHostAddress::IPv4Protocol) {
            result = address;
            break;
        }
    }
    return result;
}

QHostAddress AddressHelper::getLocalIP()
{
    return getLocalIP(QStringList());
}

/**
 * @brief AddressHelper::getLocalIP
 * @param allowedInterfaces
 * The list of allowed interface name patterns to search, in order of preference
 * (empty for all)
 * @return The first IPv4 address of the most-preferred up interface that has one,
 * or a null QHostAddress if none does. With an empty list, loopback interfaces are
 * skipped and the first up interface carrying an IPv4 address wins.
 * @note The order QNetworkInterface::allInterfaces() returns is not defined by Qt;
 * on Linux it is the netlink dump order, so an empty list gives a stable answer only
 * while the set of interfaces is stable. Pass patterns when the choice matters.
 */
QHostAddress AddressHelper::getLocalIP(const QStringList &allowedInterfaces)
{
    const bool matchAny = allowedInterfaces.count() == 0;
    QHostAddress result;

    int priority = 0x7fffffff;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces)
    {
        if((interface.flags() & QNetworkInterface::IsUp) == 0)
        {
            continue;
        }
        if(matchAny && (interface.flags() & QNetworkInterface::IsLoopBack))
        {
            continue;
        }

        int index = indexOfRegEx(allowedInterfaces, interface.name());
        if(matchAny == false && (index < 0 || index >= priority))
        {
            continue;
        }

        QHostAddress found;
        for(const QNetworkAddressEntry& address : interface.addressEntries())
        {
            if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                found = address.ip();
                break;
            }
        }
        if(found.isNull())
        {
            continue;
        }

        result = found;
        priority = index;
        if(matchAny)
        {
            break;
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
