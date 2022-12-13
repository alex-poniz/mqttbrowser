#ifndef MQTTLISTENER_H
#define MQTTLISTENER_H

#include <QObject>

class MqttListener : public QObject
{
    Q_OBJECT
public:
    explicit MqttListener(QObject *parent = nullptr);

signals:

};

#endif // MQTTLISTENER_H
