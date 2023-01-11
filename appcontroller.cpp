#include "appcontroller.h"
#include "qdebug.h"


AppController::AppController(AppData& appData, QObject *parent)
    : m_hostname("localhost"), m_port(1883), m_isConnected(false),
      m_mqttCommunicator(Mqtt::MqttCommunicator("test")),
      m_appData(appData), QObject{parent}
{
    QObject::connect(&m_mqttCommunicator, SIGNAL(onMessageReceived(const QString&, const QString&)),
                     this, SLOT(onMessageReceived(const QString&, const QString&)));
}

bool AppController::isConnected()
{
    return m_isConnected;
}

void AppController::setIsConnected(bool isConnected)
{
    if (m_isConnected == isConnected)
        return;

    m_isConnected = isConnected;
    emit isConnectedChanged();
}

QString AppController::hostname()
{
    return m_hostname;
}

void AppController::setHostname(QString hostname)
{
    if (m_hostname == hostname)
        return;

    m_hostname = hostname;
    emit hostnameChanged();
}

void AppController::setPort(int port)
{
    m_port = port;
}

bool AppController::connectToBroker()
{
    qDebug() << __PRETTY_FUNCTION__ << "hostname: " << m_hostname
             << "port: " << m_port;

    if (m_hostname.isEmpty() || m_port <= 0) {
        qDebug() << __PRETTY_FUNCTION__ << "Hostname and/or port is not specified";
        return false;
    }
    //m_pmqttClient = new NGGW_MQTT::MQTTClient(m_hostName.toStdString(), m_port);

    int connectingResult = m_mqttCommunicator.connect(m_hostname.toStdString(), m_port);

    bool ret = (connectingResult == MOSQ_ERR_SUCCESS || connectingResult == MOSQ_ERR_ERRNO);

    setIsConnected(ret);

    if (!ret) {
        qDebug() << __PRETTY_FUNCTION__ << "Cannot connect to broker. ConnectingResult: " << connectingResult;
    } else {
        m_mqttCommunicator.startMqttLoop();
        qDebug() << __PRETTY_FUNCTION__ << "Connected to broker " << m_hostname;
    }

    return ret;
}

bool AppController::subscribe(QString topic)
{
    bool res = false;
    if (m_isConnected) {
        m_mqttCommunicator.subscribe(topic.toStdString());
        res = true;
        qDebug() << __PRETTY_FUNCTION__ << "Subscribed to topic " << topic;
    }

    if (!res)
        qDebug() << __PRETTY_FUNCTION__ << "Cannot subscribe to topic " << topic;

    return res;
}

void AppController::onMessageReceived(const QString &topic, const QString &message)
{
    qDebug() << __PRETTY_FUNCTION__ << "topic: " << topic << " message: " << message;

    int topicIndex = m_appData.getIndexByTopic(topic);
    //int topicIndex = 0;

    QString responses = m_appData.m_responses[topicIndex];
    responses.append("<BR>" + message);
    m_appData.m_responses[topicIndex] = responses;

    emit m_appData.responsesChanged();
}
