/**
 *  AddressHelper
 *
 *  Some static helper methods for address resolution and parsing
 *
 *  Stephen Punak, July 08 2019
 */
#ifndef ADDRESSHELPER_H
#define ADDRESSHELPER_H
#include <QHostAddress>
#include <QString>
#include "kanoopcommon.h"

/**
 * @brief Static helper methods for network address resolution and parsing.
 */
class KANOOP_EXPORT AddressHelper
{
public:
    /**
     * @brief Parse a combined "address:port" string into its components.
     * @param addressString Combined address and port string (e.g. "192.168.1.1:8080")
     * @param address Output string receiving the address portion
     * @param port Output variable receiving the port number
     * @return true if parsing succeeded, false otherwise
     */
    static bool tryParseAddressPort(const QString& addressString, QString& address, quint16& port);

    /**
     * @brief Parse a combined "address:port" string into a QHostAddress and port.
     * @param addressString Combined address and port string (e.g. "192.168.1.1:8080")
     * @param address Output QHostAddress receiving the parsed address
     * @param port Output variable receiving the port number
     * @return true if parsing succeeded, false otherwise
     */
    static bool tryParseAddressPort(const QString& addressString, QHostAddress& address, quint16& port);

    /**
     * @brief Resolve a hostname to its IPv4 address.
     * @param hostName Hostname or IP address string to resolve
     * @return Resolved QHostAddress, or QHostAddress() on failure
     */
    static QHostAddress resolveIPv4Address(const QString& hostName);

    /**
     * @brief Get the local machine's primary IPv4 address.
     * @return The local IPv4 QHostAddress
     */
    static QHostAddress getLocalIP();

    /**
     * @brief Get the local machine's IPv4 address, filtered by allowed interface names.
     * @param allowedInterfaces List of interface name patterns to consider
     * @return The matching local IPv4 QHostAddress
     */
    static QHostAddress getLocalIP(const QStringList& allowedInterfaces);

private:
    /** @brief Return the index of the first regex in regexList that matches ifName. */
    static int indexOfRegEx(const QStringList &regexList, const QString& ifName);
};

#endif // ADDRESSHELPER_H
