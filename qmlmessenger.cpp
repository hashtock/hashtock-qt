#include "qmlmessenger.h"
#include <QDebug>
#include <QJsonObject>

QmlMessenger::QmlMessenger(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_msg = Messenger::instance();
    // Simply re-emit a signal
    connect(m_msg, &Messenger::msgEvent, this, &QmlMessenger::msgEvent);
    connect(m_msg, &Messenger::msgEvent, this, &QmlMessenger::makePrintableMessage);
}

void QmlMessenger::consumeEvent(Messenger::MessageType msg, QJsonDocument data)
{
    m_msg->consumeEvent(msg, data);
}

void QmlMessenger::makePrintableMessage(Messenger::MessageType msg, QJsonDocument data)
{
    QString printable;
    QJsonObject data_obj = data.object();

    switch (msg) {
    case Messenger::OrderAdded:
        printable = QString("New order to %1 %2 shares of %3").arg(
                            data_obj["action"].toString(),
                            QString::number(data_obj["quantity"].toDouble(), 'f', 2),
                            data_obj["hashtag"].toString());
        break;
    case Messenger::OrderCanceled:
        printable = QString("Order has been canceled");
        break;
    default:
        printable = QString("Event %1 does not have pritable form?").arg(QString(msg));
        break;
    }

    emit printableMessage(msg, data.toVariant(), printable);
}

