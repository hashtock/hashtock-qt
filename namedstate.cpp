#include "namedstate.h"

NamedState::NamedState(QString name, QObject *parent) :
    QState(parent), m_name(name)
{
}


QString NamedState::name()
{
    return m_name;
}
