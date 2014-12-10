#ifndef MESSENGER_H
#define MESSENGER_H

#include <QObject>
#include <QQmlEngine>
#include <QMutex>
#include <QJsonDocument>
// Singelton based on http://www.qtcentre.org/wiki/index.php?title=Singleton_pattern

class Messenger: public QObject
{
    Q_OBJECT
    Q_ENUMS(MessageType)
public:
    static Messenger* instance();
    static void drop();

    enum MessageType {
        Unknown = 0,
        OrderAdded,
        OrderCanceled,
    };

signals:
    void msgEvent(MessageType msg, QJsonDocument data);

public slots:
    void consumeEvent(MessageType msg, QJsonDocument data);

private:
    Messenger();

    Messenger(const Messenger &);
    Messenger& operator=(const Messenger &);

    static Messenger* m_instance;
};
Q_DECLARE_METATYPE(Messenger::MessageType)

#endif // MESSENGER_H
