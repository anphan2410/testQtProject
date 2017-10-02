#include "emitreadp.h"

emitReadP::emitReadP(UHVPVICollectorDB *database) : dbPtr(database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("emitReadP Constructed"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadP();
    });
}

void emitReadP::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter emitReadP"));
    timer.start();
}

void emitReadP::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave emitReadP"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
