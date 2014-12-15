#ifndef ANDROIDCOOKIEJAR_H
#define ANDROIDCOOKIEJAR_H

#include <QNetworkCookieJar>



class AndroidCookieJar : public QNetworkCookieJar
{
public:
    AndroidCookieJar();
    ~AndroidCookieJar();

    // QNetworkCookieJar interface
public:
    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);

private:
    void load();
    bool loaded;
};

#endif // ANDROIDCOOKIEJAR_H
