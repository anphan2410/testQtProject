#include "emitreadv.h"

emitReadV::emitReadV(UHVPVICollectorDB *database) : dbPtr(database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("emitReadV Constructed"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadV();
    });
}

void emitReadV::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter emitReadV"));
    timer.start();
}

void emitReadV::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave emitReadV"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
