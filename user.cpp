#include "user.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QQmlContext>
#include <QQmlEngine>

User::User(QQuickItem *parent) :
    QQuickItem(parent), m_name(""), m_founds(0), m_loggedin(false), m_uptodate(false)
{
}

//void User::setNetworkAccessManager(QNetworkAccessManager* nam)
//{
//    m_nam = nam;
//}

//QNetworkAccessManager* User::networkAccessManager() const
//{
//    return m_nam;
//}

QString User::name() const
{
    return m_name;
}

double User::founds() const
{
    return m_founds;
}

void User::setFounds(double value)
{
    if (m_founds == value) {
        return;
    }

    m_founds = value;
    emit foundsChanged();
}

QString User::loginUrl() const
{
    return "http://hashtock.appspot.com/api/user/";
}

bool User::loggedin() const
{
    return m_loggedin;
}

void User::setLoggedIn(bool value)
{
    if (m_loggedin == value)
    {
        return;
    }

    m_loggedin = value;
    emit loggedinChanged();

    if (m_loggedin)
    {
        emit loginSucessful();
    }
    else
    {
        emit loginRequired();
    }
}

void User::setName(QString &name)
{
    if (name == m_name) {
        return;
    }

    m_name = name;
    emit nameChanged();
}

void User::loadData()
{
    if (m_uptodate) {
        return;
    }

    qWarning() << "LOAD DATA";
    m_nam = qmlEngine(this)->networkAccessManager();

    if(!m_nam) {
        qWarning() << "Could not load user data: no NetworkAccessManager";
        return;
    }

    QUrl url("http://hashtock.appspot.com/api/user/");
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "application/json");
    QNetworkReply *resp = m_nam->get(req);
    connect(resp, SIGNAL(finished()), this, SLOT(dataReady()), Qt::QueuedConnection);
}

void User::dataReady()
{
    QNetworkReply *resp = qobject_cast<QNetworkReply *>(sender());
    int status_code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status_code / 100 == 3)
    {
        emit loginRequired();
        return;
    }

    QJsonParseError json_error;
    QByteArray data = resp->readAll();
    qDebug() << "Data:" << data;
    QJsonDocument user_doc = QJsonDocument::fromJson(data, &json_error);
    if (json_error.error != QJsonParseError::NoError)
    {
        qWarning() << "JsonError: " << json_error.errorString() << ". Data: " << data;
        return;
    }

    QJsonObject user_obj = user_doc.object();

    QJsonValue user_value = user_obj.value("id");
    QJsonValue founds_value = user_obj.value("founds");
    QString id = user_value.toString();
    float founds = founds_value.toDouble();
    setName(id);
    setFounds(founds);
    setLoggedIn(true);
    m_uptodate = true; // ToDo: add invalidation
}

void User::classBegin() {}
void User::componentComplete() { loadData(); }
