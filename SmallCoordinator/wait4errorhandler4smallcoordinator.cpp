#include "wait4errorhandler4smallcoordinator.h"

wait4ErrorHandler4SmallCoordinator::wait4ErrorHandler4SmallCoordinator(SmallCoordinatorDB *database)
    : dbPtr(database)
{
    emitErrorTimer.setParent(this);
    emitErrorTimer.setInterval(5000);
    emitErrorTimer.setSingleShot(true);
    QObject::connect(&emitErrorTimer, &QTimer::timeout, dbPtr, &SmallCoordinatorDB::emitErrorGlobalSignal);
    anIf(SmallCoordinatorDBDbgEn, anTrk("wait4ErrorHandler4SmallCoordinator Constructed"));
}

void wait4ErrorHandler4SmallCoordinator::onEntry(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("wait4ErrorHandler4SmallCoordinator Entered"));
    emitErrorTimer.start();
}

void wait4ErrorHandler4SmallCoordinator::onExit(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("Leave wait4ErrorHandler4SmallCoordinator"));
    emitErrorTimer.stop();
    dbPtr->clearError();
}
