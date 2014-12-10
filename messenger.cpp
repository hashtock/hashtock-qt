#include "messenger.h"
#include <QDebug>

Messenger* Messenger::m_instance = 0;
Messenger::Messenger(): QObject() {}

Messenger *Messenger::instance()
{
    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new Messenger();

        mutex.unlock();
    }

    return m_instance;
}

void Messenger::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

void Messenger::consumeEvent(MessageType msg, QJsonDocument data)
{
    qDebug() << "Event to emit:" << msg << ". Data:" << data;
    // ToDo: group events and fire additinal signals
    emit msgEvent(msg, data);
}
