/**
 * @brief An abstract TCP server that manages client connections in a dedicated thread.
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>
#include <QThread>

#include <Kanoop/mutexevent.h>

#include <Kanoop/utility/loggingbaseclass.h>

class TcpServerClientObject;

/**
 * @brief Abstract TCP server supporting both plain-text and TLS/SSL connections.
 *
 * Subclass TcpServer and implement createClient() to construct the appropriate
 * TcpServerClientObject for each incoming connection. The server runs its listen
 * loop in a dedicated QThread; call start() and stop() to control it.
 */
class KANOOP_EXPORT TcpServer : public QTcpServer,
                                public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct a plain-text TCP server.
     * @param serverAddress Address to listen on
     * @param serverPort Port to listen on
     */
    TcpServer(const QHostAddress& serverAddress, int serverPort);

    /**
     * @brief Construct an SSL/TLS TCP server with a single CA certificate.
     * @param serverAddress Address to listen on
     * @param serverPort Port to listen on
     * @param privateKey Server private key
     * @param localCertificate Server certificate
     * @param caCert CA certificate for client verification
     * @param verifyPeer Whether to require client certificate verification
     */
    TcpServer(const QHostAddress& serverAddress, int serverPort, const QSslKey& privateKey, const QSslCertificate& localCertificate, const QSslCertificate& caCert, bool verifyPeer = false);

    /**
     * @brief Construct an SSL/TLS TCP server with a list of CA certificates.
     * @param serverAddress Address to listen on
     * @param serverPort Port to listen on
     * @param privateKey Server private key
     * @param localCertificate Server certificate
     * @param caCerts List of CA certificates
     * @param verifyPeer Whether to require client certificate verification
     */
    TcpServer(const QHostAddress& serverAddress, int serverPort, const QSslKey& privateKey, const QSslCertificate& localCertificate, const QList<QSslCertificate>& caCerts, bool verifyPeer = false);

    /** @brief Destructor — stops the server and cleans up all client connections. */
    virtual ~TcpServer();

    /**
     * @brief Start the server and begin accepting connections.
     * @return true if the server started successfully
     */
    bool start();

    /** @brief Stop the server and close all active client connections. */
    void stop();

    /** @brief Return the server's private key (SSL mode only). */
    QSslKey privateKey() const { return _privateKey; }
    /** @brief Return the server's local certificate (SSL mode only). */
    QSslCertificate localCertificate() const { return _localCertificate; }
    /** @brief Return the list of CA certificates (SSL mode only). */
    QList<QSslCertificate> caCerts() const { return _caCerts; }
    /** @brief Return whether peer certificate verification is enabled. */
    bool verifyPeer() const { return _verifyPeer; }

    /**
     * @brief Return the time the server was started.
     * @return QDateTime of the last successful start()
     */
    QDateTime startTime() const { return _startTime; }

protected:
    /**
     * @brief Factory method called for each incoming connection.
     * @param server Pointer to this server
     * @param fd Native socket file descriptor for the new connection
     * @return Newly allocated TcpServerClientObject to manage the connection
     */
    virtual TcpServerClientObject* createClient(TcpServer* server, qintptr fd) = 0;

    /** @brief Handle a new incoming connection by delegating to createClient(). */
    virtual void incomingConnection(qintptr handle) override;

private:
    /** @brief Shared initialisation for all constructors. */
    void commonInit();

    QHostAddress _serverAddress;
    int _serverPort;

    bool _startSuccess;

    QList<TcpServerClientObject*> _clients;

    QSslKey _privateKey;
    QSslCertificate _localCertificate;
    QList<QSslCertificate> _caCerts;
    bool _verifyPeer;

    QThread _thread;
    QDateTime _startTime;
    MutexEvent _startEvent;
    MutexEvent _stopEvent;

private slots:
    /** @brief Invoked when the server thread starts. */
    void onThreadStarted();
    /** @brief Invoked when the server thread finishes. */
    void onThreadFinished();
    /** @brief Invoked when a client object signals it has finished. */
    void onClientFinished();
};

#endif // TCPSERVER_H
