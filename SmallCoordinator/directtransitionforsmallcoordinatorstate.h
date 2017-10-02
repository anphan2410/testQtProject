#ifndef DIRECTTRANSITIONFORSMALLCOORDINATORSTATE_H
#define DIRECTTRANSITIONFORSMALLCOORDINATORSTATE_H

#include <QSignalTransition>
#include <QAbstractState>
#include "smallcoordinatordb.h"

class directTransitionForSmallCoordinatorState : public QSignalTransition
{
public:
    directTransitionForSmallCoordinatorState(SmallCoordinatorDB * database, QAbstractState * destinationState);
protected:
    bool eventTest(QEvent *e) override;
};

#endif // DIRECTTRANSITIONFORSMALLCOORDINATORSTATE_H
