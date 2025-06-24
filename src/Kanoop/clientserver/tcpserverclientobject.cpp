#include "clientserver/tcpserver.h"
#include "clientserver/tcpserverclientobject.h"
#include "commonexception.h"

#include <QSslConfiguration>
#include <QSslSocket>
#include <QTcpSocket>

TcpServerClientObject::TcpServerClientObject(TcpServer *server, qintptr fd, bool useSsl) :
    _server(server), _fd(fd),
    _socket(nullptr),
    _useSsl(useSsl)
{
    TcpServerClientObject::setObjectName(TcpServerClientObject::metaObject()->className());
}

TcpServerClientObject::~TcpServerClientObject()
{
    logText(LVL_DEBUG, QString("%1 Deleting").arg(objectName()));
}

void TcpServerClientObject::writeData(const QByteArray &data)
{
    if(_socket != nullptr && _socket->isOpen()) {
        int bytesWritten;
        if((bytesWritten = _socket->write(data)) != data.length()) {
            logText(LVL_ERROR, QString("%1: Tried to write %2 but wrote %3").arg(objectName()).arg(data.length()).arg(bytesWritten));
        }
    }
}

void TcpServerClientObject::threadStarted()
{
    try
    {
        if(_useSsl) {
            startSslClient();
        }
        else {
            startClearTextClient();
        }
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Start Failure: %2").arg(objectName()).arg(e.message()));
        finishAndStop(false, e.message());
    }
}

void TcpServerClientObject::threadFinished()
{
    // allow derived class to do something here
    clientFinished();

    if(_socket != nullptr) {
        _socket->close();
        delete _socket;
    }
}

void TcpServerClientObject::startClearTextClient()
{
    _socket = new QTcpSocket;
    if(_socket->setSocketDescriptor(_fd) == false) {
        throw CommonException("Failed to set socket descriptor");
    }

    connect(_socket, &QTcpSocket::readyRead, this, &TcpServerClientObject::onReadyRead);
    connect(_socket, &QTcpSocket::disconnected, this, &TcpServerClientObject::onDisconnected);
    connect(_socket, &QTcpSocket::errorOccurred, this, &TcpServerClientObject::onErrorOcurred);

    // allow derived class to do something here
    clientStarted();
}

void TcpServerClientObject::startSslClient()
{
    QSslSocket* socket = new QSslSocket;
    if(socket->setSocketDescriptor(_fd) == false) {
        throw CommonException("Failed to set socket descriptor");
    }

    if(_server->privateKey().isNull()) {
        throw CommonException("Server has no private key set");
    }

    if(_server->localCertificate().isNull()) {
        throw CommonException("Server has no local certificate set");
    }

    if(_server->caCerts().isEmpty()) {
        throw CommonException("Server has no CA certificate(s) set");
    }

    socket->setPrivateKey(_server->privateKey());
    socket->setLocalCertificate(_server->localCertificate());
    QSslConfiguration sslConfiguration = socket->sslConfiguration();
    for(const QSslCertificate& caCert : _server->caCerts()) {
        sslConfiguration.addCaCertificate(caCert);
    }
    socket->setSslConfiguration(sslConfiguration);

    // Two-way SSL
    if(_server->verifyPeer()) {
        socket->setPeerVerifyMode(QSslSocket::VerifyPeer);
    }

    connect(socket, &QSslSocket::sslErrors, this, &TcpServerClientObject::onSslErrors);
    connect(socket, &QSslSocket::encrypted, this, &TcpServerClientObject::onEncrypted);
    connect(socket, &QSslSocket::errorOccurred, this, &TcpServerClientObject::onErrorOcurred);

    socket->startServerEncryption();

    _socket = socket;
}

void TcpServerClientObject::onReadyRead()
{
    QByteArray data = _socket->readAll();
    receivedData(data);
}

void TcpServerClientObject::onDisconnected()
{
    finishAndStop(true);
}

void TcpServerClientObject::onErrorOcurred(QAbstractSocket::SocketError errorCode)
{
    logText(LVL_WARNING, QString("%1: Error occurred %2 [%3]").arg(objectName()).arg(errorCode).arg(_socket->errorString()));
    finishAndStop(false);
}

void TcpServerClientObject::onEncrypted()
{
    connect(_socket, &QTcpSocket::readyRead, this, &TcpServerClientObject::onReadyRead);
}

void TcpServerClientObject::onSslErrors(const QList<QSslError> errors)
{
    for(const QSslError& error : errors) {
        logText(LVL_WARNING, QString("%1 [SSL Error]: %2").arg(objectName()).arg(error.errorString()));
    }
}
