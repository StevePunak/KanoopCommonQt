#include "mqtt/mqttclient.h"
#include <QMqttClient>
#include <QTimer>
#include <QUuid>
#include <commonexception.h>

MqttClient::MqttClient(QObject* parent) :
    QObject(parent),
    LoggingBaseClass("mqtt-client")
{
    MqttClient::setObjectName(MqttClient::metaObject()->className());
}

MqttClient::MqttClient(const MqttParameters& parameters, QObject* parent) :
    QObject(parent),
    LoggingBaseClass("mqtt-client"),
    _parameters(parameters)
{
    MqttClient::setObjectName(MqttClient::metaObject()->className());
}

MqttClient::~MqttClient()
{
    if(_client != nullptr) {
        delete _client;
    }
}

void MqttClient::start()
{
    try
    {
        _client = new QMqttClient;
        connect(_client, &QMqttClient::connected, this, &MqttClient::onClientConnected);
        connect(_client, &QMqttClient::disconnected, this, &MqttClient::onDisconnected);
        connect(_client, &QMqttClient::errorChanged, this, &MqttClient::onErrorChanged);
        _client->moveToThread(thread());

        _client->setHostname(_parameters.host());
        _client->setPort(_parameters.port());
        _client->setUsername(_parameters.username());
        _client->setPassword(_parameters.password());
        _client->setProtocolVersion(_parameters.protocolVersion());
        _client->setClientId(_clientId.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : _clientId);

        if(_parameters.useSsl()) {
            _sslConfig = QSslConfiguration::defaultConfiguration();
            _sslConfig.setPeerVerifyMode(_parameters.verifyPeer() ? QSslSocket::VerifyPeer : QSslSocket::VerifyNone);
            _sslConfig.setProtocol(QSsl::TlsV1_3OrLater);      // can we do 3?

            // if client cert is set, the private key must also be set
            if(_parameters.certificate().isNull() != _parameters.privateKey().isNull()) {
                throw CommonException("If client cert is set, the private key must also be set (and vice versa)");
            }

            if(_parameters.certificate().isNull() == false) {
                _sslConfig.setLocalCertificate(_parameters.certificate());
            }
            if(_parameters.caCertificate().isNull() == false) {
                _sslConfig.setCaCertificates({_parameters.caCertificate()});
            }
            if(_parameters.privateKey().isNull() == false) {
                _sslConfig.setPrivateKey(_parameters.privateKey());
            }
        }

        if(_connectAtStart) {
            doConnect();
        }
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("%1: Start exception: %2").arg(objectName()).arg(e.message()));
    }
}

void MqttClient::stop()
{
    logText(LVL_DEBUG, "Client finished");
    if(_client != nullptr) {
        delete _client;
        _client = nullptr;
    }
}

QMqttSubscription* MqttClient::subscribe(const QString& topic, uint8_t qos)
{
    QMqttSubscription* result = nullptr;
    if(isConnected()) {
        result = _client->subscribe(topic, qos);
    }
    return result;
}

int MqttClient::publish(const QString& topic, const QByteArray& payload, uint8_t qos, bool retain)
{
    if(isConnected() == false) {
        return -1;
    }
    return _client->publish(topic, payload, qos, retain);
}

void MqttClient::doConnect()
{
    if(_parameters.useSsl()) {
        _client->connectToHostEncrypted(_sslConfig);
    }
    else {
        _client->connectToHost();
    }
}

void MqttClient::onClientConnected()
{
    logText(LVL_DEBUG, "Client connected");
    _connected = true;
    connected();
    emit clientConnected();
}

void MqttClient::onDisconnected()
{
    logText(LVL_DEBUG, "Client connected");
    _connected = false;
    disconnected();
    emit clientDisconnected();
    if(_autoReconnect) {
        logText(LVL_DEBUG, QString("Client disconnected. Will reconnect in %1").arg(_autoReconnectTime.toString()));
        QTimer::singleShot(_autoReconnectTime.totalMilliseconds(), this, &MqttClient::doConnect);
    }
}

void MqttClient::onErrorChanged(QMqttClient::ClientError error)
{
    logText(LVL_DEBUG, QString("%1: Error changed to %2").arg(objectName()).arg(error));
}
