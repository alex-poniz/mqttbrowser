#ifndef APPDATA_H
#define APPDATA_H

#include <qqml.h>
#include <QObject>
#include <QString>
#include <QMap>
#include <QStringList>

class AppData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList responses READ responses NOTIFY responsesChanged)
    QML_ELEMENT

public:
    explicit AppData(QObject *parent = nullptr);

    QStringList responses() { return m_responses; };

    int getIndexByTopic(const QString& topic);

    QStringList m_responses;

public slots:
    void addTopic(const QString& topic);


signals:
    void responsesChanged();

private:
    //QMap<QString, int> m_topicIndexes;
    QList<QString> m_topics;
};

#endif // APPDATA_H
