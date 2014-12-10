#ifndef WEBKITCOOKIEJAR_H
#define WEBKITCOOKIEJAR_H

#include <QNetworkCookieJar>
#include <QtSql/QSqlDatabase>

class WebKitCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit WebKitCookieJar(QObject *parent = 0);
    ~WebKitCookieJar();

    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);

public slots:
    void reload();

signals:
    void noCookies(QUrl url);

private:
    void load();
    bool loaded;
    QSqlDatabase database;
    QString connectionName;
    QString cookieStoragePath;
};

#endif // WEBKITCOOKIEJAR_H
