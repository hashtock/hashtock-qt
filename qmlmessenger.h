#ifndef QMLMESSENGER_H
#define QMLMESSENGER_H

#include <QQuickItem>
#include "messenger.h"

class QmlMessenger : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(Messenger::MessageType)
public:
    explicit QmlMessenger(QQuickItem *parent = 0);

signals:
    void msgEvent(Messenger::MessageType msg, QJsonDocument doc);
    void printableMessage(Messenger::MessageType msg, QVariant data, QString pritable);

public slots:
    void consumeEvent(Messenger::MessageType, QJsonDocument);

private:
    Messenger *m_msg;

    void makePrintableMessage(Messenger::MessageType, QJsonDocument);
};

#endif // QMLMESSENGER_H
