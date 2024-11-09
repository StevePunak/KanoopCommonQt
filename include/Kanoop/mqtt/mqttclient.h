#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H
#include <QObject>

#include <Kanoop/utility/abstractthreadclass.h>
#include <Kanoop/mqtt/mqttparameters.h>

class QMqttClient;
class MqttClient : public AbstractThreadClass
{
    Q_OBJECT
public:
    MqttClient(const MqttParameters& parameters);
    virtual ~MqttClient();

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

    QMqttSubscription* subscribe(const QString& topic, uint8_t qos = 0);

    QMqttClient* client() const { return _client; }

protected:
    virtual void threadStarted() override;
    virtual void threadFinished() override;

private:
    MqttParameters _parameters;
    bool _connectAtStart = true;
    bool _autoReconnect = true;
    TimeSpan _autoReconnectTime = TimeSpan::fromSeconds(1);
    QString _clientId;

    QMqttClient* _client = nullptr;

signals:
    void clientConnected();
    void clientDisconnected();

private slots:
    void onClientConnected();
    void onDisconnected();
};

#endif // MQTTCLIENT_H
