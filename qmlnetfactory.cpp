#include "qmlnetfactory.h"
#include "webkitcookiejar.h"
#include "androidcookiejar.h"

#include "gaenetworkaccessmanager.h"

QNetworkAccessManager*
QMLNetFactory::create(QObject *parent)
{
    GAENetworkAccessManager *nam = new GAENetworkAccessManager(parent);
//    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
//    nam->setCookieJar(new WebKitCookieJar);
//    nam->setCookieJar(new AndroidCookieJar);

    return nam;
}
