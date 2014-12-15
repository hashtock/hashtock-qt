#ifndef QMLNETFACTORY_H
#define QMLNETFACTORY_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class QMLNetFactory : public QQmlNetworkAccessManagerFactory
{
public:
    virtual QNetworkAccessManager *create(QObject *parent);

};

#endif // QMLNETFACTORY_H
