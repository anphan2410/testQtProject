#include "updatelocaldatabase.h"

updateLocalDatabase::updateLocalDatabase(piLocalDBWorkerVarSet *VarSet, QState *parent) :
    QState(parent), VarSetPtr(VarSet)
{
    checkControlTask.setParent(this);
    checkControlTask.setInterval(1000);
    checkControlTask.setSingleShot(true);
    QObject::connect(&checkControlTask, &QTimer::timeout, VarSetPtr, &piLocalDBWorkerVarSet::requestControlTableCheck);
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("updateLocalDatabase Constructed"));
}

void updateLocalDatabase::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Enter updateLocalDatabase"));
    checkControlTask.start();
    if (VarSetPtr->prioritizedBuffer.size())
    {
        VarSetPtr->updateLocalDatabase();
        qApp->processEvents();
        emit VarSetPtr->directTransitionRequest(QStringLiteral("updateOnlineDatabase"));
        VarSetPtr->takeOutFirstOfMostPrioritizedGlobalSignals();
    }
    else
    {
        anIf(piLocalDBWorkerVarSetDbgEn, anInfo("Buffer Is Empty"));
    }
}

void updateLocalDatabase::onExit(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Leave updateLocalDatabase"));
    checkControlTask.stop();
}
