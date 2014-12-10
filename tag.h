#ifndef BANK_H
#define BANK_H

#include "hashtockapi.h"
#include "orderapi.h"
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonArray>

class Tag
{
public:
    Tag(QString name, float value, float in_bank);
    static Tag fromJson(QJsonObject json);

    QString hashtag() const;
    float value() const;
    float in_bank() const;

private:
    QString m_name;
    float m_value;
    float m_in_bank;
};

class TagModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TagModel(QObject *parent = 0);

    enum TagRoles {
        NameRole = Qt::UserRole + 1,
        ValueRole,
        InBankRole,
    };

public slots:
    void loadData();
    void buy(QString hashtag, float quantity);

private slots:
    void processData(QJsonDocument);

// QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QList<Tag> m_tags;
    HashTockApi *m_api;
    OrderApi *m_order;
};

#endif // BANK_H
