#ifndef ADDRESSHELPER_H
#define ADDRESSHELPER_H
#include <QHostAddress>
#include <QString>
#include "kanoopcommon.h"

class KANOOP_EXPORT AddressHelper
{
public:
    static bool tryParseAddressPort(const QString& addressString, QString& address, quint16& port);
    static bool tryParseAddressPort(const QString& addressString, QHostAddress& address, quint16& port);
    static QHostAddress getLocalIP();
    static QHostAddress getLocalIP(const QStringList& allowedInterfaces);

private:
    AddressHelper();
    static int indexOfRegEx(const QStringList &regexList, const QString& ifName);
};

#endif // ADDRESSHELPER_H
