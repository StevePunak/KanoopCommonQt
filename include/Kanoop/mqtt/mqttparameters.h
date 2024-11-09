#ifndef MQTTPARAMETERS_H
#define MQTTPARAMETERS_H
#include <QString>
#include <QMqttClient>

class MqttParameters
{
public:
    MqttParameters() {}
    MqttParameters(const QString& host, int port = 1883) :
        _host(host), _port(port) {}

    QString host() const { return _host; }
    void setHost(const QString& value) { _host = value; }

    int port() const { return _port; }
    void setPort(int value) { _port = value; }

    QString username() const { return _username; }
    void setUsername(const QString& value) { _username = value; }

    QString password() const { return _password; }
    void setPassword(const QString& value) { _password = value; }

    QMqttClient::ProtocolVersion protocolVersion() const { return _protocolVersion; }
    void setProtocolVersion(QMqttClient::ProtocolVersion value) { _protocolVersion = value; }

private:
    QString _host;
    int _port = 1883;
    QString _username;
    QString _password;
    QMqttClient::ProtocolVersion _protocolVersion = QMqttClient::MQTT_5_0;
};

#endif // MQTTPARAMETERS_H
