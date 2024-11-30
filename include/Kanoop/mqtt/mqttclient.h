#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H
#include <QObject>

#include <Kanoop/timespan.h>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/mqtt/mqttparameters.h>

class MqttClient : public QObject,
                   public LoggingBaseClass
{
    Q_OBJECT
public:
    MqttClient(QObject* parent = nullptr);
    MqttClient(const MqttParameters& parameters, QObject* parent = nullptr);
    virtual ~MqttClient();

    virtual void start();
    virtual void stop();

    MqttParameters parameters() const { return _parameters; }
    void setParameters(const MqttParameters& value) { _parameters = value; }

    bool connectAtStart() const { return _connectAtStart; }
    void setConnectAtStart(bool value) { _connectAtStart = value; }

    bool autoReconnect() const { return _autoReconnect; }
    void setAutoReconnect(bool value) { _autoReconnect = value; }

    TimeSpan autoReconnectTime() const { return _autoReconnectTime; }
    void setAutoReconnectTime(const TimeSpan& value) { _autoReconnectTime = value; }

    QString clientId() const { return _clientId; }
    void setClientId(const QString& value) { _clientId = value; }

    bool isConnected() const { return _client != nullptr && _connected; }

    QMqttSubscription* subscribe(const QString& topic, uint8_t qos = 0);
    int publish(const QString& topic, const QByteArray& payload, uint8_t qos = 0, bool retain = false);

protected:
    QMqttClient* client() const { return _client; }

    virtual void connected() {}
    virtual void disconnected() {}

private:
    MqttParameters _parameters;
    bool _connectAtStart = true;
    bool _autoReconnect = true;
    TimeSpan _autoReconnectTime = TimeSpan::fromSeconds(1);
    QString _clientId;
    bool _connected = false;

    QSslConfiguration _sslConfig;
    QMqttClient* _client = nullptr;

signals:
    void clientConnected();
    void clientDisconnected();

private slots:
    void doConnect();
    void onClientConnected();
    void onDisconnected();
    void onErrorChanged(QMqttClient::ClientError error);
};

#endif // MQTTCLIENT_H
