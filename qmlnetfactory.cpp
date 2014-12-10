#include "qmlnetfactory.h"
#include "webkitcookiejar.h"

QNetworkAccessManager*
QMLNetFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
    nam->setCookieJar(new WebKitCookieJar);

    return nam;
}
