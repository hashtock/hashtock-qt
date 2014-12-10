#include "hashtockapi.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>

HashTockApi::HashTockApi(QString endpoint, QObject *parent) :
    QObject(parent),
    m_endpoint(endpoint), m_nam(0), m_loggedin(false), m_request_in_progress(false)
{
    m_msg = Messenger::instance();
    connect(m_msg, &Messenger::msgEvent, this, &HashTockApi::msgEvent);
}

void HashTockApi::setNetworkAccessManager(QNetworkAccessManager *value)
{
    if (value == NULL || value == m_nam)
        return;

    m_nam = value;
}

QNetworkAccessManager *HashTockApi::networkAccessManager()
{
    if (m_nam == NULL) {
        QQmlEngine *engine = qmlEngine(this);
        if (engine == NULL) {
            engine = qmlEngine(this->parent());
        }

        if (engine == NULL) {
            qWarning() << "Could not get access to Engine at all";
            return NULL;
        }

        m_nam = engine->networkAccessManager();
    }

    return m_nam;
}

QString HashTockApi::loginUrl() const
{
    return "http://hashtock.appspot.com/api/user/";
}

bool HashTockApi::loggedin() const
{
    return m_loggedin;
}

QString HashTockApi::endpoint()
{
    return m_endpoint;
}

void HashTockApi::setEndpoint(QString value)
{
    m_endpoint = value;
}

void HashTockApi::setRequestInProgress(bool value)
{
    if (m_request_in_progress == value)
    {
        return;
    }

    m_request_in_progress = value;
    emit requestInProgressChanged();
}

bool HashTockApi::requestInProgress()
{
    return m_request_in_progress;
}

bool HashTockApi::isError(QJsonDocument doc)
{
    // ToDo: This is rather incorrect needs work
    return doc.isNull();
}

bool HashTockApi::uptodate() const
{
    return m_uptodate;
}

void HashTockApi::loadData()
{
    if (uptodate() || m_request_in_progress) {
        return;
    }
    setRequestInProgress(true);

    qDebug() << "LOAD DATA for " << endpoint();
    m_nam = networkAccessManager();

    if(!m_nam) {
        qWarning() << "Could not load data: no NetworkAccessManager";
        return;
    }

    QUrl url(endpoint());
    QNetworkRequest req(url);
    req.setRawHeader("Accept", "application/json");
    QNetworkReply *resp = m_nam->get(req);
    QObject::connect(resp, SIGNAL(finished()), this, SLOT(dataFetched()), Qt::QueuedConnection);
}

void HashTockApi::sendData(QJsonDocument doc)
{
    qDebug() << "Send data to " << endpoint() << ":" << doc;

    m_nam = networkAccessManager();

    if(!m_nam) {
        qWarning() << "Could not send data: no NetworkAccessManager";
        return;
    }

    QUrl url(endpoint());
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    qDebug() << "HEAD:" << req.header(QNetworkRequest::ContentTypeHeader);
    QNetworkReply *resp = m_nam->post(req, doc.toJson());
    QObject::connect(resp, SIGNAL(finished()), this, SLOT(dataSentResponse()), Qt::QueuedConnection);
}

void HashTockApi::deleteData(QString resource_id)
{
    qDebug() << "To delete from " << endpoint() << ":" << resource_id;

    m_nam = networkAccessManager();

    if(!m_nam) {
        qWarning() << "Could not send data: no NetworkAccessManager";
        return;
    }

    if (!resource_id.endsWith("/"))
    {
        resource_id += "/";
    }

    QUrl url(endpoint() + resource_id);
    qDebug() << "URL:" << url;
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Accept", "application/json");
    qDebug() << "HEAD:" << req.header(QNetworkRequest::ContentTypeHeader);
    QNetworkReply *resp = m_nam->deleteResource(req);
    QObject::connect(resp, SIGNAL(finished()), this, SLOT(dataDeletedResponse()), Qt::QueuedConnection);
}

void HashTockApi::dataFetched()
{
    setRequestInProgress(false);
    QJsonDocument json_doc = processJsonReplay();

    if (isError(json_doc)) {
        return;
    }

    setUpToDate(true);
    emit dataReady(json_doc);
}

void HashTockApi::dataSentResponse()
{
    qDebug() << "dataSentResponse";
    QJsonDocument json_doc = processJsonReplay();
    dataSent(json_doc);

    if (isError(json_doc)) {
        qDebug() << "Error sending data:" << json_doc;
        emit dataSentError(json_doc);
        return;
    }

    qDebug() << "Sending data ok:" << json_doc;
    emit dataSentOk(json_doc);
}

void HashTockApi::dataDeletedResponse()
{
    qDebug() << "dataDeletedResponse";
    QJsonDocument json_doc = processJsonReplay(true);
    dataDeleted(json_doc);

    if (isError(json_doc)) {
        qDebug() << "Error deleting data:" << json_doc;
        emit dataDeletedError(json_doc);
        return;
    }

    emit dataDeletedOk(json_doc);
}

QJsonDocument HashTockApi::processJsonReplay(bool is_delete)
{
    QNetworkReply *resp = qobject_cast<QNetworkReply *>(sender());
    int status_code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Status code:" << status_code;
    if (status_code / 100 == 3)
    {
        emit loginRequired();
        return QJsonDocument();
    } if (status_code == 204 && is_delete) {
        return deletedResource(resp->url());
    }

    QJsonParseError json_error;
    QByteArray data = resp->readAll();
    qDebug() << "Data:" << data;
    QJsonDocument json_doc = QJsonDocument::fromJson(data, &json_error);
    if (json_error.error != QJsonParseError::NoError)
    {
        qWarning() << "JsonError: " << json_error.errorString() << ". Data: " << data;
        return QJsonDocument();
    }

    return json_doc;
}

void HashTockApi::setUpToDate(bool value)
{
    qDebug() << "setUpToDate called with: " << value << " (" << endpoint() << ")";
    if (value == m_uptodate) return;

    m_uptodate = value;
    emit uptodateChanged(m_uptodate);
}

QJsonDocument HashTockApi::deletedResource(QUrl request_url)
{
    Q_UNUSED(request_url);
    qWarning() << "Could not identify resource that has been deleted.";
    return QJsonDocument();
}
