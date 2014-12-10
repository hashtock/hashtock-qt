#include "order.h"

OrderModel::OrderModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_orderapi = new OrderApi(this);
    connect(m_orderapi, SIGNAL(dataReady(QJsonDocument)), this, SLOT(processData(QJsonDocument)), Qt::QueuedConnection);
    connect(m_orderapi, SIGNAL(dataDeletedOk(QJsonDocument)), this, SLOT(orderDeletedOk(QJsonDocument)), Qt::QueuedConnection);
}

void OrderModel::loadData()
{
    m_orderapi->loadData();
}

void OrderModel::cancel(QString uuid)
{
    m_orderapi->cancel(uuid);
}

void OrderModel::processData(QJsonDocument data)
{
    if (data.isEmpty() || data.isNull()) {
        qWarning() << "No data to process:" << data;
        return;
    }

    QJsonArray to_add;
    if (data.isArray())
    {
        to_add = data.array();
    }
    else
    {
        to_add.append(data.object());
    }

    beginInsertRows(QModelIndex(), m_orders.size(), m_orders.size() + to_add.size()-1);
    foreach (const QJsonValue obj, to_add)
    {
        Order t = Order::fromJson(obj.toObject());
        m_orders.append(t);
    }
    endInsertRows();
}

void OrderModel::orderDeletedOk(QJsonDocument doc)
{
    if (doc.isEmpty() || doc.isNull()) {
        return;
    }

    QJsonObject obj = doc.object();

    if (!obj.contains("id")) {
        qWarning() << "Delete info obj does not contain uuid. Doc: " << doc;
        return;
    }

    QString uuid = obj["id"].toString();
    int idx = 0;
    foreach (const Order order, m_orders) {
        if (order.uuid() == uuid)
        {
            beginRemoveRows(QModelIndex(), idx, idx);
            m_orders.removeAt(idx);
            endRemoveRows();
            qDebug() << "Remove idx:" << idx;

            break;
        }
        idx++;
    }
}

int OrderModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_orders.size();
}

QVariant OrderModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_orders.count())
        return QVariant();

    const Order &order = m_orders[index.row()];
    if (role == Action) return order.action();
    else if (role == BankOrder) return order.bank_order();
    else if (role == Hashtag) return order.hashtag();
    else if (role == Quantity) return order.quantity();
    else if (role == UUID) return order.uuid();
    else if (role == Complete) return order.complete();
    return QVariant();
}

QHash<int, QByteArray> OrderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Action] = "action";
    roles[BankOrder] = "bank_order";
    roles[Hashtag] = "hashtag";
    roles[Quantity] = "quantity";
    roles[UUID] = "uuid";
    roles[Complete] = "complete";
    return roles;
}

Order::Order(QString action, bool bank_order, QString hashtag, float quantity, QString uuid, bool complete):
    m_action(action), m_bank_order(bank_order), m_hashtag(hashtag),
    m_quantity(quantity), m_uuid(uuid), m_complete(complete)
{}

Order Order::fromJson(QJsonObject json)
{
    QString action = json.value("action").toString();
    bool bank_order = json.value("bank_order").toBool();
    QString hashtag = json.value("hashtag").toString();
    float quantity = json.value("quantity").toDouble();
    QString uuid = json.value("uuid").toString();
    bool complete = json.value("complete").toBool();

    return Order(action, bank_order, hashtag, quantity, uuid, complete);
}

QString Order::action() const { return m_action; }
bool Order::bank_order() const { return m_bank_order; }
QString Order::hashtag() const { return m_hashtag; }
float Order::quantity() const { return m_quantity; }
QString Order::uuid() const { return m_uuid; }
bool Order::complete() const { return m_complete; }
