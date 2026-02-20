/**
 * @brief Abstract base class for objects that handle a single TCP server client connection.
 */
#ifndef TCPSERVERCLIENTOBJECT_H
#define TCPSERVERCLIENTOBJECT_H
#include <QAbstractSocket>
#include <QSslError>

#include <Kanoop/utility/abstractthreadclass.h>

class TcpServer;
class QTcpSocket;

/**
 * @brief Manages the lifetime of a single accepted TCP (or SSL/TLS) client socket.
 *
 * Each accepted connection is wrapped in a TcpServerClientObject running in its own
 * thread. Subclasses must implement receivedData() to process incoming bytes, and
 * may override clientStarted() and clientFinished() for lifecycle hooks.
 */
class KANOOP_EXPORT TcpServerClientObject : public AbstractThreadClass
{
    Q_OBJECT
public:
    /** @brief Destructor — disconnects and cleans up the socket. */
    virtual ~TcpServerClientObject();

    /**
     * @brief Send data to the connected client.
     * @param data Bytes to transmit
     */
    void writeData(const QByteArray& data);

protected:
    /**
     * @brief Construct a client object for an accepted connection.
     * @param server Owning TcpServer
     * @param fd Native socket file descriptor for the accepted connection
     * @param useSsl Whether to negotiate SSL/TLS on this socket
     */
    TcpServerClientObject(TcpServer* server, qintptr fd, bool useSsl = false);

    /** @brief Called on the client thread immediately after the connection is established. */
    virtual void clientStarted() {}

    /** @brief Called on the client thread just before the connection is torn down. */
    virtual void clientFinished() {}

    /**
     * @brief Called whenever data arrives from the client.
     * @param data Received bytes
     */
    virtual void receivedData(const QByteArray& data) = 0;

    /**
     * @brief Return the underlying socket for this client.
     * @return Pointer to the QTcpSocket (may also be a QSslSocket in SSL mode)
     */
    QTcpSocket* socket() const { return _socket; }

    /**
     * @brief Return the owning TcpServer.
     * @return Pointer to the parent TcpServer
     */
    TcpServer* server() const { return _server; }

private:
    /** @brief Invoked by the thread framework when the thread starts. */
    virtual void threadStarted() override final;
    /** @brief Invoked by the thread framework when the thread finishes. */
    virtual void threadFinished() override final;

    /** @brief Set up the socket for plain-text operation. */
    void startClearTextClient();
    /** @brief Set up the socket for SSL/TLS operation. */
    void startSslClient();

    TcpServer* _server;
    qintptr _fd;

    QTcpSocket* _socket;
    bool _useSsl;

private slots:
    /** @brief Called when data is available to read from the socket. */
    void onReadyRead();
    /** @brief Called when the remote end disconnects. */
    void onDisconnected();
    /** @brief Called when a socket error occurs. */
    void onErrorOcurred(QAbstractSocket::SocketError errorCode);
    /** @brief Called when the SSL handshake completes. */
    void onEncrypted();
    /** @brief Called when SSL errors are reported. */
    void onSslErrors(const QList<QSslError> errors);
};

#endif // TCPSERVERCLIENTOBJECT_H
