#include "wait4v.h"

wait4V::wait4V(UHVPVICollectorDB *database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("wait4V Constructed"));
    timer.setParent(this);
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHVPVICollectorStateDbgEn, anWarn("Wait For V Timed Out !"));
        database->saveV();
    });
}

void wait4V::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter wait4V"));
    timer.start();
}

void wait4V::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave wait4V"));
    timer.stop();
}
