#include "coordinateglobalsignals.h"

coordinateGlobalSignals::coordinateGlobalSignals(SmallCoordinatorDB *database, QState *parent) :
    QState(parent),
    dbPtr(database)
{
    timer.setParent(this);
    timer.setInterval(30000);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){
        GlobalSignal resumeAllAutoCollectors;
        resumeAllAutoCollectors.Type = QVariant::fromValue(SmallCoordinatorDB::resumeAllCollectors);
        resumeAllAutoCollectors.DstStrs.append(SmallCoordinatorObjName);
        dbPtr->addOneGlobalSignal(resumeAllAutoCollectors);
    });
    anIf(SmallCoordinatorDBDbgEn, anTrk("coordinateGlobalSignals Constructed"));
}

void coordinateGlobalSignals::onEntry(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("coordinateGlobalSignals Entered"));
    if (dbPtr->prioritizedBuffer.size())
    {
        dbPtr->executeGlobalSignals();
        qApp->processEvents();
        emit dbPtr->requestDirectTransition("coordinateGlobalSignals");
        dbPtr->takeOutFirstOfMostPrioritizedGlobalSignals();
    }
    else
    {
        anIf(SmallCoordinatorDBDbgEn, anInfo("Buffer Is Empty"));
        timer.start();
    }
}

void coordinateGlobalSignals::onExit(QEvent *)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("Leave coordinateGlobalSignals"));
}

