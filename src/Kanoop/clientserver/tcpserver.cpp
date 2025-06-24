#include "clientserver/tcpserver.h"
#include "clientserver/tcpserverclientobject.h"

TcpServer::TcpServer(const QHostAddress &serverAddress, int serverPort) :
    QTcpServer(),
    LoggingBaseClass("tcpserver"),
    _serverAddress(serverAddress),
    _serverPort(serverPort),
    _startSuccess(false)
{
    TcpServer::commonInit();
}

TcpServer::TcpServer(const QHostAddress &serverAddress, int serverPort, const QSslKey &privateKey, const QSslCertificate &localCertificate, const QSslCertificate &caCert, bool verifyPeer) :
    QTcpServer(),
    LoggingBaseClass("tcpserver"),
    _serverAddress(serverAddress),
    _serverPort(serverPort),
    _startSuccess(false),
    _privateKey(privateKey), _localCertificate(localCertificate),
    _verifyPeer(verifyPeer)
{
    _caCerts << caCert;
    TcpServer::commonInit();
}

TcpServer::TcpServer(const QHostAddress &serverAddress, int serverPort, const QSslKey &privateKey, const QSslCertificate &localCertificate, const QList<QSslCertificate> &caCerts, bool verifyPeer) :
    QTcpServer(),
    LoggingBaseClass("tcpserver"),
    _serverAddress(serverAddress),
    _serverPort(serverPort),
    _startSuccess(false),
    _privateKey(privateKey),
    _localCertificate(localCertificate),
    _caCerts(caCerts),
    _verifyPeer(verifyPeer)
{
    TcpServer::commonInit();
}

void TcpServer::commonInit()
{
    TcpServer::setObjectName(TcpServer::metaObject()->className());

    connect(&_thread, &QThread::started, this, &TcpServer::onThreadStarted);
    connect(&_thread, &QThread::finished, this, &TcpServer::onThreadFinished);
    moveToThread(&_thread);
}

TcpServer::~TcpServer()
{
    for(TcpServerClientObject* client : _clients) {
        client->stop();
        delete client;
    }
}

bool TcpServer::start()
{
    _thread.start();
    _startTime = QDateTime::currentDateTimeUtc();
    if((_startEvent.wait(TimeSpan::fromSeconds(5))) == false) {
        logText(LVL_ERROR, QString("%1 failed to start - %2").arg(objectName()).arg(errorString()));
        _startSuccess = false;
    }
    return _startSuccess;
}

void TcpServer::stop()
{
    for(TcpServerClientObject* client : _clients) {
        client->stop();
        delete client;
    }
    _clients.clear();
    _thread.quit();
    if(_stopEvent.wait(TimeSpan::fromSeconds(5)) == false) {
        logText(LVL_ERROR, QString("%1 failed to stop").arg(objectName()));
    }
}

void TcpServer::incomingConnection(qintptr handle)
{
    logText(LVL_INFO, QString("%1: %2 on %3").arg(objectName()).arg(__FUNCTION__).arg(handle));
    TcpServerClientObject* client = createClient(this, handle);
    if(client != nullptr) {
        client->start();
        connect(client, &TcpServerClientObject::finished, this, &TcpServer::onClientFinished);
        _clients.append(client);
    }
}

void TcpServer::onThreadStarted()
{
    logText(LVL_INFO, QString("%1: Started").arg(objectName()));
    if((_startSuccess = listen(_serverAddress, _serverPort)) == true) {
        logText(LVL_DEBUG, QString("%1: Listening on %2:%3").arg(objectName()).arg(_serverAddress.toString()).arg(_serverPort));
    }
    _startEvent.set();
}

void TcpServer::onThreadFinished()
{
    logText(LVL_INFO, QString("%1: Finished").arg(objectName()));
    _stopEvent.set();
}

void TcpServer::onClientFinished()
{
    logText(LVL_INFO, QString("%1: Client Finished").arg(objectName()));
    TcpServerClientObject* client = static_cast<TcpServerClientObject*>(sender());
    _clients.removeAll(client);
    delete client;

}
