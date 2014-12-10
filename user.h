#ifndef USER_H
#define USER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QQuickItem>

class User : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(double founds READ founds NOTIFY foundsChanged)
    Q_PROPERTY(QString loggedin READ loggedin NOTIFY loggedinChanged)
    Q_PROPERTY(QString loginUrl READ loginUrl CONSTANT)
public:
    explicit User(QQuickItem *parent = 0);

    void setNetworkAccessManager(QNetworkAccessManager*);
    QNetworkAccessManager* networkAccessManager() const;

    QString name() const;
    double founds() const;
    QString loginUrl() const;
    bool loggedin() const;

public slots:
    void loadData();

signals:
    void nameChanged();
    void foundsChanged();
    void loggedinChanged();
    void loginRequired();
    void loginSucessful();

private slots:
    void dataReady();

// QQmlParserStatus interface
public:
    virtual void classBegin();
    virtual void componentComplete();

private:
    QNetworkAccessManager* m_nam;
    QString m_name;
    double m_founds;
    bool m_loggedin;
    bool m_uptodate;

    void setName(QString &name);
    void setFounds(double value);
    void setLoggedIn(bool value);
};

#endif // USER_H
