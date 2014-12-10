#ifndef QMLNETFACTORY_H
#define QMLNETFACTORY_H

#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class QMLNetFactory : public QQmlNetworkAccessManagerFactory
{
//    Q_OBJECT
public:
//    explicit QMLNetFactory(QObject *parent = 0);
    virtual QNetworkAccessManager *create(QObject *parent);

};

#endif // QMLNETFACTORY_H
