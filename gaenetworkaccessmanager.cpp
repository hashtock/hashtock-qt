#include "gaenetworkaccessmanager.h"

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QUrlQuery>

GAENetworkAccessManager::GAENetworkAccessManager(QObject *parent):
    QNetworkAccessManager(parent), gae_authenticated(false)
{

}

QUrl GAENetworkAccessManager::get_app_url(QUrl req_url)
{
    QUrl auth_url;
    auth_url.setScheme(req_url.scheme());
    auth_url.setHost(req_url.host());
    return auth_url;
}

bool GAENetworkAccessManager::auth_gae(QUrl req_url)
{
    QUrl app_url(get_app_url(req_url));
    if (authenticated.contains(app_url.toString()))
    {
        return true;
    }

    QAndroidJniObject tokenObj = QAndroidJniObject::callStaticObjectMethod("org/hashtock/auth/Auth", "get_auth_key", "()Ljava/lang/String;");
    QString token = tokenObj.toString();

    if (token.isEmpty())
    {
        qWarning() << "Auth token not available!";
        return false;
    }

    // Build auth url: "http://APP.appspot.com/_ah/login?continue=http://localhost&auth=TOKEN
    QUrl url(app_url);
    url.setPath("/_ah/login");

    QUrlQuery query;
    query.addQueryItem("continue", "http://localhost");
    query.addQueryItem("auth", token);
    url.setQuery(query);

    QNetworkRequest auth_request(url);
    QNetworkReply *auth_reply = this->get(auth_request);

    // Wait for request to finish (blocking)
    QEventLoop loop;
    connect(auth_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    authenticated.insert(app_url.toString());
    return true;
}

bool GAENetworkAccessManager::needs_auth(QUrl url)
{
    if (!url.host().endsWith(".appspot.com"))
    {
        return false;
    }

    if (url.path().startsWith("/_ah/login"))
    {
        return false;
    }

    return true;
}

QNetworkReply *GAENetworkAccessManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    QUrl req_url = request.url();
    qDebug() << "Req:" << req_url;
    if (needs_auth(req_url))
    {
        auth_gae(req_url);
    }

    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
