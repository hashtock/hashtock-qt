#include "tag.h"
#include <QDebug>

TagModel::TagModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_api = new HashTockApi("http://hashtock.appspot.com/api/tag/", this);
    connect(m_api, SIGNAL(dataReady(QJsonDocument)), this, SLOT(processData(QJsonDocument)), Qt::QueuedConnection);

    m_order = new OrderApi(this);
}

void TagModel::loadData()
{
    m_api->loadData();
}

void TagModel::buy(QString hashtag, float quantity)
{
    m_order->buy(hashtag, quantity, true);
}

void TagModel::processData(QJsonDocument data)
{
    if (!data.isArray() || data.isEmpty() || data.isNull()) {
        qWarning() << "No data to process:" << data;
        return;
    }

    beginInsertRows(QModelIndex(), m_tags.size(), m_tags.size() + data.array().size()-1);
    foreach (const QJsonValue obj, data.array())
    {
        Tag t = Tag::fromJson(obj.toObject());
        m_tags.append(t);
    }
    endInsertRows();
}

int TagModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tags.size();
}

QVariant TagModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_tags.count())
        return QVariant();

    const Tag &tag = m_tags[index.row()];
    if (role == NameRole) return tag.hashtag();
    else if (role == ValueRole) return tag.value();
    else if (role == InBankRole) return tag.in_bank();
    return QVariant();
}

QHash<int, QByteArray> TagModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "hashtag";
    roles[ValueRole] = "value";
    roles[InBankRole] = "in_bank";
    return roles;
}

Tag::Tag(QString name, float value, float in_bank):
    m_name(name), m_value(value), m_in_bank(in_bank)
{}

Tag Tag::fromJson(QJsonObject json)
{
    QString name = json.value("hashtag").toString();
    float value = json.value("value").toDouble();
    float in_bank = json.value("in_bank").toDouble();

    return Tag(name, value, in_bank);
}

QString Tag::hashtag() const { return m_name; }
float Tag::value() const { return m_value; }
float Tag::in_bank() const { return m_in_bank; }
