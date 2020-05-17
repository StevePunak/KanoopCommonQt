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

private:
    AddressHelper();
};

#endif // ADDRESSHELPER_H
