#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>
#include <QString>
#include <QMap>

class AppData : public QObject
{
    Q_OBJECT
public:
    explicit AppData(QObject *parent = nullptr);
    QMap<QString, QString> m_responsesMap;


signals:

};

#endif // APPDATA_H
