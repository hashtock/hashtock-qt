#ifndef ORDERAPI_H
#define ORDERAPI_H

#include "hashtockapi.h"
#include <QAbstractListModel>
#include <QJsonDocument>

class OrderApi : public HashTockApi
{
    Q_OBJECT
public:
    explicit OrderApi(QObject *parent = 0);

public slots:
    void buy(QString hashtag, float quantity, bool from_bank);
    void cancel(QString uuid);

    // HashTockApi interface
protected:
    virtual QJsonDocument deletedResource(QUrl);
    virtual void dataSent(QJsonDocument);
    virtual void dataDeleted(QJsonDocument);
    virtual void msgEvent(Messenger::MessageType msg, QJsonDocument data);
};

#endif // ORDERAPI_H
