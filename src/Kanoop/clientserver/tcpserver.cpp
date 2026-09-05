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
    reapClients(takeClients());
}

QList<TcpServerClientObject*> TcpServer::takeClients()
{
    QMutexLocker l(&_clientsLock);
    QList<TcpServerClientObject*> clients;
    clients.swap(_clients);
    return clients;
}

void TcpServer::reapClients(const QList<TcpServerClientObject*>& clients)
{
    for(TcpServerClientObject* client : clients) {
        // ⚠ Disconnect before stop(): stop() joins the client's thread, and that join emits
        // finished() -- a queued delivery that would arrive with the client already deleted.
        disconnect(client, nullptr, this, nullptr);

        // ⚠ Subclasses wire this server to the client in createClient(). That signal is
        // delivered during the join below, in a slot that may reach back through server() --
        // already part-destroyed when the reap runs from ~TcpServer.
        disconnect(this, nullptr, client, nullptr);

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
    // ⚠ The roster is only final once _thread's event loop is gone -- incomingConnection()
    // runs there and appends. Reaping first leaks a client accepted in the gap.
    _thread.quit();
    if(_stopEvent.wait(TimeSpan::fromSeconds(5)) == false) {
        logText(LVL_ERROR, QString("%1 failed to stop").arg(objectName()));
    }

    // _stopEvent fires from inside QThread::finished, before the thread is finished. Callers
    // delete this server on return, and ~QThread on a running thread is fatal.
    _thread.wait();

    reapClients(takeClients());
}

void TcpServer::incomingConnection(qintptr handle)
{
    logText(LVL_INFO, QString("%1: %2 on %3").arg(objectName()).arg(__FUNCTION__).arg(handle));
    TcpServerClientObject* client = createClient(this, handle);
    if(client != nullptr) {
        // ⚠ On the roster, and connected, before it is started. A client that finishes
        // immediately would otherwise emit finished() while absent from the roster, and the
        // guard in onClientFinished() would read that as "already reaped" and leak it.
        _clientsLock.lock();
        _clients.append(client);
        _clientsLock.unlock();

        connect(client, &TcpServerClientObject::finished, this, &TcpServer::onClientFinished);
        client->start();
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
    TcpServerClientObject* client = static_cast<TcpServerClientObject*>(sender());

    // ⚠ The roster decides, and nothing here dereferences the pointer before it does.
    // finished() is queued from the client's own thread, so a delivery can outlive the
    // client: stop() may already have disconnected, stopped and deleted it, and Qt still
    // places this call with a DANGLING sender(). Deleting that a second time reads a freed
    // vtable. Presence on the roster is the only trustworthy liveness test -- a pointer
    // comparison, never a read.
    _clientsLock.lock();
    const bool wasOnRoster = _clients.removeAll(client) > 0;
    _clientsLock.unlock();

    if(wasOnRoster == true) {
        logText(LVL_INFO, QString("%1: Client Finished").arg(objectName()));
        delete client;
    }
}

#include "Kanoop/clientserver/moc_tcpserver.cpp"
