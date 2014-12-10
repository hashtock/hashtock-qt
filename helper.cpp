#include "helper.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStandardPaths>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QNetworkCookie>
#include <QNetworkAccessManager>
#include "networkaccessmanager.h"
//#include "cookiejar.h"
#include "webkitcookiejar.h"

Helper::Helper(QObject *parent) :
    QObject(parent)
{
}

void Helper::onLoadMe() {
//    NetworkAccessManager *s_networkAccessManager = new NetworkAccessManager();
//    s_networkAccessManager->setCookieJar(new CookieJar);
//    engine->networkAccessManager()->setCookieJar(new WebKitCookieJar);

    QUrl url("http://hashtock.appspot.com/api/?format=json");
    QNetworkRequest req(url);
//    QNetworkReply *resp = s_networkAccessManager->get(req);
    QNetworkReply *resp = engine->networkAccessManager()->get(req);
    connect(resp, SIGNAL(finished()), this, SLOT(slotReadReady()), Qt::UniqueConnection);
}

void Helper::slotReadReady()
{
    QNetworkReply *resp = qobject_cast<QNetworkReply *>(sender());
    qDebug() << "Rep: " << resp->readAll();
    qDebug() << "Err: " << resp->error();
    qDebug() << "Err String: " << resp->errorString();
    qDebug() << "Status code " << resp->attribute(QNetworkRequest::HttpStatusCodeAttribute);
}
