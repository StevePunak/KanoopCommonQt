#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>
#include <QThread>

#include <Kanoop/mutexevent.h>

#include <Kanoop/utility/loggingbaseclass.h>

class TcpServerClientObject;
class KANOOP_EXPORT TcpServer : public QTcpServer,
                                public LoggingBaseClass
{
    Q_OBJECT
public:
    TcpServer(const QHostAddress& serverAddress, int serverPort);
    TcpServer(const QHostAddress& serverAddress, int serverPort, const QSslKey& privateKey, const QSslCertificate& localCertificate, const QSslCertificate& caCert, bool verifyPeer = false);
    TcpServer(const QHostAddress& serverAddress, int serverPort, const QSslKey& privateKey, const QSslCertificate& localCertificate, const QList<QSslCertificate>& caCerts, bool verifyPeer = false);
    virtual ~TcpServer();

    bool start();
    void stop();

    // SSL
    QSslKey privateKey() const { return _privateKey; }
    QSslCertificate localCertificate() const { return _localCertificate; }
    QList<QSslCertificate> caCerts() const { return _caCerts; }
    bool verifyPeer() const { return _verifyPeer; }

    QDateTime startTime() const { return _startTime; }

protected:
    virtual TcpServerClientObject* createClient(TcpServer* server, qintptr fd) = 0;
    virtual void incomingConnection(qintptr handle) override;

private:
    void commonInit();

    QHostAddress _serverAddress;
    int _serverPort;

    bool _startSuccess;

    QList<TcpServerClientObject*> _clients;

    // SSL
    QSslKey _privateKey;
    QSslCertificate _localCertificate;
    QList<QSslCertificate> _caCerts;
    bool _verifyPeer;

    QThread _thread;
    QDateTime _startTime;
    MutexEvent _startEvent;
    MutexEvent _stopEvent;

private slots:
    void onThreadStarted();
    void onThreadFinished();
    void onClientFinished();
};

#endif // TCPSERVER_H
