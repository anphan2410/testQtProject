#include "wait4readyworkers.h"

wait4ReadyWorkers::wait4ReadyWorkers(QState *parent) : QState(parent)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("wait4ReadyWorkers Constructed"));
}

void wait4ReadyWorkers::onEntry(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("wait4ReadyWorkers Entered"));
}

void wait4ReadyWorkers::onExit(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("Leave wait4ReadyWorkers"));
}
