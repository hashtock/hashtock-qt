#ifndef STATEMASTER_H
#define STATEMASTER_H

#include <QObject>
#include <QStateMachine>
#include <QAbstractTransition>
#include <QDebug>
#include <QPair>
#include <QMultiMap>

class StateMaster : public QObject
{
    Q_OBJECT
    Q_ENUMS(Event)
    Q_PROPERTY(QString state READ currentState NOTIFY stateChanged)
public:
    explicit StateMaster(QObject *parent = 0);

    enum Event {
        Start = QEvent::User + 1,
        Splash,
        LoginRequired,
        LoginSuccessful,
        LoginCompleted,

        GoToUserView,
        GoToBankView,
        GoToOrdersView,
        GoToHistoryView,
    };

    QString currentState();
signals:
    void stateChanged();

public slots:
    void consumeEvent(Event event);

private:
    typedef QPair<QString, Event> Transition;

    QStateMachine machine;

    QState *stInit;
    QState *stSplash;
    QState *stLogin;
    QState *stUser;

    QMultiMap<QString, Transition> m_transitions;
    QMap<QString, QState*> m_states;

    QString m_current_state;

    QState *getState(QString state_name);

private slots:
    void stateEntered();
};

class AppEventTransition: public QAbstractTransition
{
public:
    AppEventTransition(StateMaster::Event event)
        : QAbstractTransition(),
          m_event(event)
    {
    }

protected:
    virtual bool eventTest(QEvent *event)
    {
        return (event->type() == QEvent::Type(m_event));
    }

    virtual void onTransition(QEvent *) {}

private:
    StateMaster::Event m_event;
};

class StateEvents : public QObject
{
    Q_OBJECT
    Q_ENUMS(StateMaster::Event)
};

#endif // STATEMASTER_H
