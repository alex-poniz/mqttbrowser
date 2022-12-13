#include "mqttcommunicator.h"
#include "mqttlistener.h"

#include <QDebug>

using namespace Mqtt;

MqttCommunicator::MqttCommunicator(QString clientName, QObject *parent) :
    m_mqttLoopThread(this),
    MQTTClient(clientName.toStdString())
{
    this->setDelegate(this);
    //m_mqttLoopThread.start();
}

void MqttCommunicator::on_message_received(const struct mosquitto_message* message)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (message != nullptr)
    {
        emit onMessageReceived(QString(message->topic), QString((char *)message->payload));
    }
}

void MqttCommunicator::on_connected(int rc)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_connected_with_flags(int rc, int flags)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_disconnected(int rc)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_publish(int id)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_subscribed(int mid, int qos_count, const int* granted_qos)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_unsubscribed(int mid)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void MqttCommunicator::on_log(int level, const char* str)
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << str;
}
