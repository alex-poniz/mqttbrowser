#include "appdata.h"

AppData::AppData(QObject *parent)
    : QObject{parent}
{
    //m_responses << "one" << "two" << "three";
}

int AppData::getIndexByTopic(const QString& topic)
{
    return m_topics.indexOf(topic);
}

void AppData::addTopic(const QString& topic)
{
    m_topics.push_back(topic);
    //int topicIndex = getIndexByTopic(topic);
    m_responses.push_back("");

    emit responsesChanged();
}
