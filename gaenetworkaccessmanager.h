#ifndef GAENETWORKACCESSMANAGER_H
#define GAENETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class GAENetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    GAENetworkAccessManager(QObject *parent = 0);

private:
    bool gae_authenticated;
    QSet<QString> authenticated;

    QUrl get_app_url(QUrl req_url);
    bool auth_gae(QUrl req_url);
    bool needs_auth(QUrl url);

protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // GAENETWORKACCESSMANAGER_H
