#include "statemaster.h"

#include <QDebug>
#include <QEventTransition>

StateMaster::StateMaster(QObject *parent) :
    QObject(parent)
{
    /**
    States:
    - splashState
    - loginState
    - userState
    - bankState
    - orderState
    - historyState

    Trasnforms:
    - splashState -> loginState (by LoginRequired)
    -             -> userState (by LoginSuccessful)
    - loginState -> splashState (by LoginCompleted)
    - userState -> bankState (by GoToBankView)
                -> orderState (by GoToOrderView)
                -> historyState (by GoToHistoryView)
    - bankState -> userState (by GoToUserView)
                -> orderState (by GoToOrderView)
                -> historyState (by GoToHistoryView)
    - orderState -> userState (by GoToUserView)
                 -> bankState (by GoToBankView)
                 -> historyState (by GoToHistoryView)
    - historyState -> userState (by GoToUserView)
                   -> bankState (by GoToBankView)
                   -> orderState (by GoToOrderView)
    */
    m_transitions.insert("splashState", Transition("loginState", LoginRequired));
    m_transitions.insert("splashState", Transition("userState", LoginSuccessful));

    m_transitions.insert("loginState", Transition("splashState", LoginCompleted));

    m_transitions.insert("userState", Transition("bankState", GoToBankView));
    m_transitions.insert("userState", Transition("orderState", GoToOrdersView));
    m_transitions.insert("userState", Transition("historyState", GoToHistoryView));

    m_transitions.insert("bankState", Transition("userState", GoToUserView));
    m_transitions.insert("bankState", Transition("orderState", GoToOrdersView));
    m_transitions.insert("bankState", Transition("historyState", GoToHistoryView));

    m_transitions.insert("orderState", Transition("userState", GoToUserView));
    m_transitions.insert("orderState", Transition("bankState", GoToBankView));
    m_transitions.insert("orderState", Transition("historyState", GoToHistoryView));

    m_transitions.insert("historyState", Transition("userState", GoToUserView));
    m_transitions.insert("historyState", Transition("bankState", GoToBankView));
    m_transitions.insert("historyState", Transition("orderState", GoToOrdersView));

    // Init all of the states
    QList<QString> ukeys = m_transitions.uniqueKeys();
    QList<QString>::iterator key_it;
    for (key_it = ukeys.begin(); key_it != ukeys.end(); ++key_it )
    {
        QState *state = getState(*key_it);
        machine.addState(state);
        connect(state, SIGNAL(entered()), this, SLOT(stateEntered()));
    }

    // Build connections between states
    QMultiMap<QString, Transition>::Iterator it;
    for (it = m_transitions.begin(); it != m_transitions.end(); it++ )
    {
        QState *src_state = getState(it.key());
        QState *dst_state = getState(it->first);
        Event event = it->second;

        AppEventTransition *event_trasition = new AppEventTransition(event);
        event_trasition->setTargetState(dst_state);
        src_state->addTransition(event_trasition);
    }

    machine.setInitialState(getState("splashState"));
//    machine.setInitialState(getState("userState"));
    machine.start();
}

QString StateMaster::currentState()
{
    return m_current_state;
}

void StateMaster::consumeEvent(Event event)
{
//    qDebug() << "SM: consumeEvent" << event;
    QEvent *evt = new QEvent(QEvent::Type(event));
    machine.postEvent(evt);
//    qDebug() << "SM: config" << machine.configuration();
}

QState *StateMaster::getState(QString state_name)
{
    if (!m_states.contains(state_name)) {
        QState *state = new QState();
        state->setObjectName(state_name);
        m_states.insert(state_name, state);
    }

    return m_states[state_name];
}

void StateMaster::stateEntered()
{
//    qDebug() << "stateEntered";
    QState *state = qobject_cast<QState *>(sender());
    QString name = state->objectName();

//    qDebug() << "stateEntered name" << name;

    if (name == m_current_state) {
        return;
//        qDebug() << "stateEntered no change";
    }

    m_current_state = name;
    emit stateChanged();
}
