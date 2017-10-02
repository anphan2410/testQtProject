#ifndef DIRECTTRANSITIONFORUHVPVICOLLECTORSTATE_H
#define DIRECTTRANSITIONFORUHVPVICOLLECTORSTATE_H

#include <QSignalTransition>
#include <QAbstractState>
#include <QStateMachine>
#include "uhvpvicollectordb.h"

class directTransitionForUHVPVICollectorState : public QSignalTransition
{
public:
    directTransitionForUHVPVICollectorState(UHVPVICollectorDB * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITIONFORUHVPVICOLLECTORSTATE_H
