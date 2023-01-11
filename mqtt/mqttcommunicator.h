#ifndef MQTTCOMMUNICATOR_H
#define MQTTCOMMUNICATOR_H

#include <QString>
#include <QThread>
#include <QDebug>

#include "mqtt/mqttclient.h"

namespace Mqtt {

class MqttLoopThread : public QThread
{
    Q_OBJECT
    void run() override {
        m_pClient->loop_forever();
        /*
        while (!m_stop) {
            int ret = m_pClient->loop();
            if (ret != 0) {
                qDebug() << __PRETTY_FUNCTION__ << "Error: " << ret;
                break;
            }
        } */

        qDebug() << __PRETTY_FUNCTION__ << "Loop exited!";
    };

public:
    MqttLoopThread(MQTTClient* pClient) : m_pClient(pClient), m_stop(false) {};
    ~MqttLoopThread()
    {
        this->stop();
    };

    void stop()
    {
        m_stop = true;
    };

private:
    MQTTClient* m_pClient;
    //TODO: use smth like std::atomic_bool instead of ordinary bool
    bool m_stop;
};

class MqttCommunicator : public MQTTClient, IMqttCallbacks
{
    Q_OBJECT

public:
    explicit MqttCommunicator(QString clientName, QObject *parent = nullptr);
    //using QObject::connect;
    using MQTTClient::connect;
    void startMqttLoop() { m_mqttLoopThread.start(); };

signals:
    void onMessageReceived(const QString& topic, const QString& message);

protected:
    virtual void on_message_received(const struct mosquitto_message* message);
    virtual void on_connected(int rc);
    virtual void on_connected_with_flags(int rc, int flags);
    virtual void on_disconnected(int rc);
    virtual void on_publish(int id);
    virtual void on_subscribed(int mid, int qos_count, const int* granted_qos);
    virtual void on_unsubscribed(int mid);
    virtual void on_log(int level, const char* str);
private:
    MqttLoopThread m_mqttLoopThread;
    //MQTTClient m_mqttClient;
};

} // namespace

#endif // MQTTCOMMUNICATOR_H
