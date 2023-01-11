#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <qqml.h>

#include "appdata.h"
#include "mqtt/mqttcommunicator.h"

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected WRITE setIsConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)

    //Q_PROPERTY(QString updatedTopic READ updatedTopic WRITE setUpdatedTopic NOTIFY updatedTopicChanged)
    //Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    QML_ELEMENT

public:
    explicit AppController(AppData &appData, QObject *parent = nullptr);

    bool isConnected();
    void setIsConnected(bool isConnected);

    QString hostname();
    void setHostname(QString hostname);

public slots:
    void setPort(int port);
    bool connectToBroker();
    bool subscribe(QString topic);
    void onMessageReceived(const QString& topic, const QString& message);

signals:
    void isConnectedChanged();
    void hostnameChanged();
    void responsesChanged();

private:
    QString m_hostname;
    int     m_port;
    bool    m_isConnected;

    Mqtt::MqttCommunicator m_mqttCommunicator;
    AppData &m_appData;
};

#endif // APPCONTROLLER_H
