#include "directtransition4pilocaldbworkerstate.h"

directTransition4piLocalDBWorkerState::directTransition4piLocalDBWorkerState(piLocalDBWorkerVarSet *database, QAbstractState *destinationState) :
    QSignalTransition(database, &piLocalDBWorkerVarSet::directTransitionRequest)
{
    this->setTargetState(destinationState);
}

bool directTransition4piLocalDBWorkerState::eventTest(QEvent *e)
{
    if (!(QSignalTransition::eventTest(e) && this->targetState()))
        return false;
    QStateMachine::SignalEvent * se = static_cast<QStateMachine::SignalEvent *>(e);
    return (se->arguments().at(0).toString() == this->targetState()->objectName());
}

