#include "webkitcookiejar.h"

#include <QStandardPaths>
#include <QSettings>
#include <QtSql/QSqlQuery>
#include <QNetworkCookie>

WebKitCookieJar::WebKitCookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
    , loaded(false)
{
    connectionName = "webkit_coonection";
    cookieStoragePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) +
                        QLatin1String("/.QtWebKit") +
                        QLatin1String("/cookies.db");
    database = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), connectionName);
    database.setDatabaseName(cookieStoragePath);
}

WebKitCookieJar::~WebKitCookieJar()
{
    if (database.isOpen())
        database.close();
}

void WebKitCookieJar::load()
{
    if (loaded)
        return;

    if (!database.open()) {
        qWarning("Can't open cookie database");
        return;
    }

    QList<QNetworkCookie> cookies;
    QSqlQuery sqlQuery(database);
    sqlQuery.prepare(QLatin1String("SELECT cookie FROM cookies"));
    sqlQuery.exec();
    while (sqlQuery.next())
        cookies.append(QNetworkCookie::parseCookies(sqlQuery.value(0).toByteArray()));

    if (cookies.empty()) {
        emit noCookies(QUrl());
        return;
    }

    setAllCookies(cookies);
    loaded = true;
}

void WebKitCookieJar::reload()
{
    loaded = false;
    load();
}

QList<QNetworkCookie> WebKitCookieJar::cookiesForUrl(const QUrl &url) const
{
    WebKitCookieJar *that = const_cast<WebKitCookieJar*>(this);
    that->load();

    QList<QNetworkCookie> cookies = QNetworkCookieJar::cookiesForUrl(url);

    if (cookies.empty())
        emit that->noCookies(url);

    return cookies;
}

bool WebKitCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    load();
    return true;
}
