#include "directtransitionforsmallcoordinatorstate.h"

directTransitionForSmallCoordinatorState::directTransitionForSmallCoordinatorState(SmallCoordinatorDB *database, QAbstractState *destinationState) :
    QSignalTransition(database, &SmallCoordinatorDB::requestDirectTransition)
{
    this->setTargetState(destinationState);
}

bool directTransitionForSmallCoordinatorState::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}
