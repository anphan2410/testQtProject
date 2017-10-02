#ifndef WAIT4ERRORHANDLER4SMALLCOORDINATOR_H
#define WAIT4ERRORHANDLER4SMALLCOORDINATOR_H

#include <QState>
#include <QTimer>
#include "smallcoordinatordb.h"

class wait4ErrorHandler4SmallCoordinator : public QState
{
public:
    wait4ErrorHandler4SmallCoordinator(SmallCoordinatorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer emitErrorTimer;
    SmallCoordinatorDB * dbPtr;
};

#endif // WAIT4ERRORHANDLER4SMALLCOORDINATOR_H
