#include "idleuhvpvicollector.h"

idleUHVPVICollector::idleUHVPVICollector(UHVPVICollectorDB *database) : dbPtr(database)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("idleUHVPVICollector Constructed"));
}

void idleUHVPVICollector::onEntry(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Enter idleUHVPVICollector"));
    if (!(dbPtr->isReady))
    {
        if (dbPtr->connectDatabase())
        {
            GlobalSignal iamReady;
            iamReady.Type = QVariant::fromValue(UHVPVICollectorDB::readyToWork);
            iamReady.Data = QVariant::fromValue(this->machine()->objectName());
            emit dbPtr->Out(iamReady);
        }
    }
}

void idleUHVPVICollector::onExit(QEvent *)
{
    anIf(UHVPVICollectorStateDbgEn, anTrk("Leave idleUHVPVICollector"));
}
