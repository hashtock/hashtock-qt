#ifndef NAMEDSTATE_H
#define NAMEDSTATE_H

#include <QState>

class NamedState : public QState
{
    Q_OBJECT
public:
    explicit NamedState(QString name, QObject *parent = 0);

    QString name();
signals:

public slots:

private:
    QString m_name;
};

#endif // NAMEDSTATE_H
