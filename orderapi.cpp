#include "orderapi.h"

OrderApi::OrderApi(QObject *parent) :
    HashTockApi("http://hashtock.appspot.com/api/order/", parent)
{
}

void OrderApi::buy(QString hashtag, float quantity, bool from_bank)
{
    QJsonObject post_data;
    post_data["action"] = "buy";
    post_data["bank_order"] = from_bank;
    post_data["hashtag"] = hashtag;
    post_data["quantity"] = quantity;

    sendData(QJsonDocument(post_data));
}

void OrderApi::cancel(QString uuid)
{
    deleteData(uuid);
}

QJsonDocument OrderApi::deletedResource(QUrl req)
{
    QJsonObject obj;
    QString url = req.url();

    url = url.remove(endpoint());
    QStringList parts = url.split("/", QString::SkipEmptyParts);
    if (parts.size() == 1) {
        obj["id"] = parts.first();
    }

    return QJsonDocument(obj);
}

void OrderApi::dataSent(QJsonDocument data)
{
    setUpToDate(false);
    m_msg->consumeEvent(Messenger::OrderAdded, data);
}

void OrderApi::dataDeleted(QJsonDocument data)
{
    setUpToDate(false);
    m_msg->consumeEvent(Messenger::OrderCanceled, data);
}

void OrderApi::msgEvent(Messenger::MessageType msg, QJsonDocument data)
{
    if (msg == Messenger::OrderAdded) {
        emit dataReady(data);
    }
}
