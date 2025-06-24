#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QList>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <Kanoop/utility/abstractthreadclass.h>

class KANOOP_EXPORT UdpServer : public AbstractThreadClass
{
    Q_OBJECT
public:
    UdpServer(const QHostAddress& serverAddress, int serverPort);

    void writeDatagram(const QByteArray& datagram, const QHostAddress& address, int port);
    void writeDatagram(const QNetworkDatagram& datagram);

private:
    virtual void threadStarted() override;

    QHostAddress _serverAddress;
    int _serverPort;

    QUdpSocket* _socket;

    QList<QNetworkDatagram> _transmitQueue;
    QMutex _queueLock;

signals:
    void datagramReceived(const QNetworkDatagram& datagram);

private slots:
    void readPendingDatagrams();
    void serviceTransmitQueue();
};

#endif // UDPSERVER_H
