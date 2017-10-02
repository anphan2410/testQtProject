#ifndef DIRECTTRANSITION4PILOCALDBWORKERSTATE_H
#define DIRECTTRANSITION4PILOCALDBWORKERSTATE_H

#include <QSignalTransition>
#include <QAbstractState>
#include <QStateMachine>
#include "pilocaldbworkervarset.h"

class directTransition4piLocalDBWorkerState : public QSignalTransition
{
public:
    directTransition4piLocalDBWorkerState(piLocalDBWorkerVarSet * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITION4PILOCALDBWORKERSTATE_H
