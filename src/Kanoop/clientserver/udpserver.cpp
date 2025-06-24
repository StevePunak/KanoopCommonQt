#include "clientserver/udpserver.h"
#include "commonexception.h"

#include <QTimer>

UdpServer::UdpServer(const QHostAddress &serverAddress, int serverPort) :
    AbstractThreadClass("udpsrv"),
    _serverAddress(serverAddress), _serverPort(serverPort),
    _socket(nullptr)
{
    UdpServer::setObjectName(UdpServer::metaObject()->className());
}

void UdpServer::writeDatagram(const QByteArray &datagram, const QHostAddress &address, int port)
{
    writeDatagram(QNetworkDatagram(datagram, address, port));
}

void UdpServer::writeDatagram(const QNetworkDatagram &datagram)
{
    QMutexLocker l(&_queueLock);
    _transmitQueue.append(datagram);
    QTimer::singleShot(0, this, &UdpServer::serviceTransmitQueue);
}

void UdpServer::threadStarted()
{
    logText(LVL_INFO, QString("%1: Started").arg(objectName()));
    try
    {
        _socket = new QUdpSocket(this);
        if(_socket->bind(_serverAddress, _serverPort) == false) {
            throw CommonException(QString("Failed to bind: %1").arg(_socket->errorString()));
        }

        connect(_socket, &QUdpSocket::readyRead, this, &UdpServer::readPendingDatagrams);
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Start Exception: %2").arg(objectName()).arg(e.message()));
        finishAndStop(false, e.message());
    }
}

void UdpServer::readPendingDatagrams()
{
    while(_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = _socket->receiveDatagram();
        emit datagramReceived(datagram);
    }
}

void UdpServer::serviceTransmitQueue()
{
    QMutexLocker l(&_queueLock);
    while(_transmitQueue.count() > 0) {
        QNetworkDatagram datagram = _transmitQueue.takeFirst();
        _socket->writeDatagram(datagram);
    }
}
