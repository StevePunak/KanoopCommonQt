#ifndef TCPSERVERCLIENTOBJECT_H
#define TCPSERVERCLIENTOBJECT_H
#include <QAbstractSocket>
#include <QSslError>

#include <Kanoop/utility/abstractthreadclass.h>

class TcpServer;
class QTcpSocket;
class KANOOP_EXPORT TcpServerClientObject : public AbstractThreadClass
{
    Q_OBJECT
public:
    virtual ~TcpServerClientObject();

    void writeData(const QByteArray& data);

protected:
    TcpServerClientObject(TcpServer* server, qintptr fd, bool useSsl = false);

    virtual void clientStarted() {}
    virtual void clientFinished() {}
    virtual void receivedData(const QByteArray& data) = 0;

    QTcpSocket* socket() const { return _socket; }

    TcpServer* server() const { return _server; }

private:
    virtual void threadStarted() override final;
    virtual void threadFinished() override final;

    void startClearTextClient();
    void startSslClient();

    TcpServer* _server;
    qintptr _fd;

    QTcpSocket* _socket;
    bool _useSsl;

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOcurred(QAbstractSocket::SocketError errorCode);
    void onEncrypted();
    void onSslErrors(const QList<QSslError> errors);
};

#endif // TCPSERVERCLIENTOBJECT_H
