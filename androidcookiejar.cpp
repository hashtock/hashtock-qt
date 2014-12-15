#include "androidcookiejar.h"

#include <QDebug>

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QNetworkCookie>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

AndroidCookieJar::AndroidCookieJar()
{

}

AndroidCookieJar::~AndroidCookieJar()
{

}


QList<QNetworkCookie> AndroidCookieJar::cookiesForUrl(const QUrl &url) const
{
    AndroidCookieJar *that = const_cast<AndroidCookieJar*>(this);
    that->load();

    QList<QNetworkCookie> cookies = QNetworkCookieJar::cookiesForUrl(url);

    return cookies;
}

bool AndroidCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    load();
    return true;
}

void AndroidCookieJar::load()
{
    qDebug() << "load cookies";
//    if (loaded)
//        return;

    QAndroidJniObject tokenObj = QAndroidJniObject::callStaticObjectMethod("org/hashtock/auth/AccountList", "get_auth_key", "()Ljava/lang/String;");
    QString token = tokenObj.toString();

    QNetworkAccessManager netman;
    QNetworkRequest netRequest(QUrl("http://hashtock.appspot.com/_ah/login?continue=http://localhost&auth=" + token));
    QNetworkReply *netReply = netman.get(netRequest);

    QEventLoop loop;
    connect(netReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    int status_code = netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Status code:" << status_code;

    QNetworkRequest netRequest_data(QUrl("http://hashtock.appspot.com/api/user/?format=json"));
    QNetworkReply *netReply_data = netman.get(netRequest_data);

    connect(netReply_data, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    int status_code_data = netReply_data->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Status code Data:" << status_code_data;
    qDebug() << netReply_data->readAll();

//    qDebug() << "Cookie in QT: " << cookieValue.toString();

//    QByteArray name("ACSID");
//    QString value = cookieValue.toString();
//    qDebug() << "set cookie: ACSID to " << value.toLatin1();
//    QNetworkCookie androidCookie(name, value.toLatin1());

    QList<QNetworkCookie> cookies;
//    cookies.append(androidCookie);

    setAllCookies(cookies);
    loaded = !cookies.isEmpty();
}
