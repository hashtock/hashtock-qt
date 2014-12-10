#ifndef ORDER_H
#define ORDER_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "orderapi.h"

class Order
{
public:
    Order(QString action, bool bank_order, QString hashtag, float quantity, QString uuid, bool complete);
    static Order fromJson(QJsonObject json);

    QString action() const;
    bool bank_order() const;
    QString hashtag() const;
    float quantity() const;
    QString uuid() const;
    bool complete() const;

private:
    QString m_action;
    bool m_bank_order;
    QString m_hashtag;
    float m_quantity;
    QString m_uuid;
    bool m_complete;
};

class OrderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit OrderModel(QObject *parent = 0);

    enum TagRoles {
        Action = Qt::UserRole + 1,
        BankOrder,
        Hashtag,
        Quantity,
        UUID,
        Complete,
    };

signals:

public slots:
    void loadData();
    void cancel(QString uuid);

private slots:
    void processData(QJsonDocument);
    void orderDeletedOk(QJsonDocument);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QList<Order> m_orders;
    OrderApi *m_orderapi;
};

#endif // ORDER_H
