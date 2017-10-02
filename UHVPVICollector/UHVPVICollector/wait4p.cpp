#include "wait4p.h"

wait4P::wait4P(UHVPVICollectorDB *database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("wait4P Constructed"));
    timer.setParent(this);
    timer.setInterval(database->waitIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        anIf(UHVPVICollectorStateDbgEn, anWarn("Wait For P Timed Out !"));
        database->saveP();
    });
}

void wait4P::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter wait4P"));
    timer.start();
}

void wait4P::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave wait4P"));
    timer.stop();
}
