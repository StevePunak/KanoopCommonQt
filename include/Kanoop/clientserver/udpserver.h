/**
 * @brief A UDP server that receives datagrams and queues outgoing datagrams.
 */
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QList>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <Kanoop/utility/abstractthreadclass.h>

/**
 * @brief Receives and transmits UDP datagrams in a dedicated thread.
 *
 * UdpServer binds to the configured address and port on start(). Incoming
 * datagrams are delivered via the datagramReceived() signal. Outgoing datagrams
 * are queued and sent asynchronously via writeDatagram().
 */
class KANOOP_EXPORT UdpServer : public AbstractThreadClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct a UdpServer bound to the given address and port.
     * @param serverAddress Address to bind to
     * @param serverPort Port to bind to
     */
    UdpServer(const QHostAddress& serverAddress, int serverPort);

    /**
     * @brief Queue a datagram for transmission to the specified address and port.
     * @param datagram Payload bytes to send
     * @param address Destination address
     * @param port Destination port
     */
    void writeDatagram(const QByteArray& datagram, const QHostAddress& address, int port);

    /**
     * @brief Queue a fully-specified datagram for transmission.
     * @param datagram QNetworkDatagram including destination address and port
     */
    void writeDatagram(const QNetworkDatagram& datagram);

private:
    /** @brief Bind the UDP socket when the thread starts. */
    virtual void threadStarted() override;

    QHostAddress _serverAddress;
    int _serverPort;

    QUdpSocket* _socket;

    QList<QNetworkDatagram> _transmitQueue;
    QMutex _queueLock;

signals:
    /**
     * @brief Emitted when a datagram is received.
     * @param datagram The received datagram including sender address and port
     */
    void datagramReceived(const QNetworkDatagram& datagram);

private slots:
    /** @brief Called when incoming datagrams are available to read. */
    void readPendingDatagrams();
    /** @brief Called to drain and send the outgoing transmit queue. */
    void serviceTransmitQueue();
};

#endif // UDPSERVER_H
