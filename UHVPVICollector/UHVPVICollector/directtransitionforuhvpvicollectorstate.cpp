#include "directtransitionforuhvpvicollectorstate.h"

directTransitionForUHVPVICollectorState::directTransitionForUHVPVICollectorState(UHVPVICollectorDB *database, QAbstractState *destinationState) :
    QSignalTransition(database, &UHVPVICollectorDB::directTransitionRequest)
{
    this->setTargetState(destinationState);
}

bool directTransitionForUHVPVICollectorState::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}

