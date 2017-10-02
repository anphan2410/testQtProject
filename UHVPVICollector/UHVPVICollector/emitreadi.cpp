#include "emitreadi.h"

emitReadI::emitReadI(UHVPVICollectorDB *database) : dbPtr(database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("emitReadI Constructed"));
    timer.setParent(this);
    timer.setInterval(database->breakIntervalMSecs);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [database](){
        database->emitReadI();
    });
}

void emitReadI::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter emitReadI"));
    timer.start();
}

void emitReadI::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave emitReadI"));
    timer.stop();
    dbPtr->previousReadState = this->objectName();
}
