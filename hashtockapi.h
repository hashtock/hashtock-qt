#ifndef HASHTOCKAPI_H
#define HASHTOCKAPI_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include "messenger.h"

class HashTockApi: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loggedin READ loggedin NOTIFY loggedinChanged)
    Q_PROPERTY(QString loginUrl READ loginUrl CONSTANT)
    Q_PROPERTY(bool requestInProgress READ requestInProgress NOTIFY requestInProgressChanged)
    Q_PROPERTY(bool uptodate READ uptodate NOTIFY uptodateChanged)
public:
    explicit HashTockApi(QString endpoint, QObject *parent = 0);

    void setNetworkAccessManager(QNetworkAccessManager*);
    QNetworkAccessManager* networkAccessManager();

    QString loginUrl() const;
    bool loggedin() const;

    QString endpoint();
    void setEndpoint(QString value);

    QJsonDocument lastData();
    void setRequestInProgress(bool);
    bool requestInProgress();

    bool isError(QJsonDocument);

    bool uptodate() const;

signals:
    void loggedinChanged();
    void uptodateChanged(bool);
    void loginRequired();
    void loginSucessful();
    void dataReady(QJsonDocument);
    void requestInProgressChanged();
    void dataSentOk(QJsonDocument);
    void dataSentError(QJsonDocument);
    void dataDeletedError(QJsonDocument);
    void dataDeletedOk(QJsonDocument);

public slots:
    void loadData();
    void sendData(QJsonDocument);
    void deleteData(QString);

private slots:
    void dataFetched();
    void dataSentResponse();
    void dataDeletedResponse();

private:
    QString m_endpoint;
    QNetworkAccessManager* m_nam;
    bool m_loggedin;
    bool m_uptodate; // synchronize with messanger? :/
    QJsonDocument m_last_data;
    bool m_request_in_progress;

    QJsonDocument processJsonReplay(bool is_delete=false);

protected:
    Messenger *m_msg;

    void setUpToDate(bool);

    virtual QJsonDocument deletedResource(QUrl);

    virtual void dataSent(QJsonDocument){}
    virtual void dataDeleted(QJsonDocument){}
    virtual void msgEvent(Messenger::MessageType, QJsonDocument){}
};

#endif // HASHTOCKAPI_H
