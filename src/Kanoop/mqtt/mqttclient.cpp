#include "mqtt/mqttclient.h"
#include <QMqttClient>
#include <QTimer>
#include <commonexception.h>

MqttClient::MqttClient(const MqttParameters& parameters) :
    AbstractThreadClass("mqtt-client"),
    _parameters(parameters)
{
}

MqttClient::~MqttClient()
{
    if(_client != nullptr) {
        delete _client;
    }
}

void MqttClient::threadStarted()
{
    try
    {
        _client = new QMqttClient;
        connect(_client, &QMqttClient::connected, this, &MqttClient::onClientConnected);
        connect(_client, &QMqttClient::disconnected, this, &MqttClient::onDisconnected);
        _client->moveToThread(thread());

        _client->setHostname(_parameters.host());
        _client->setPort(_parameters.port());
        _client->setUsername(_parameters.username());
        _client->setPassword(_parameters.password());
        _client->setProtocolVersion(_parameters.protocolVersion());

        if(_connectAtStart) {
            _client->connectToHost();
        }
    }
    catch(const CommonException& e)
    {
        logText(LVL_ERROR, QString("Start exception: %1").arg(e.message()));
        finishAndStop(false, e.message());
    }
}

void MqttClient::threadFinished()
{
    logText(LVL_DEBUG, "Client finished");
    if(_client != nullptr) {
        delete _client;
        _client = nullptr;
    }
}

void MqttClient::onClientConnected()
{
    logText(LVL_DEBUG, "Client connected");
    emit clientConnected();
}

void MqttClient::onDisconnected()
{
    emit clientDisconnected();
    if(_autoReconnect) {
        logText(LVL_DEBUG, QString("Client disconnected. Will reconnect in %1").arg(_autoReconnectTime.toString()));
        QTimer::singleShot(_autoReconnectTime.totalMilliseconds(), _client, qOverload<>(&QMqttClient::connectToHost));
    }
}
