#include "setissentcolumnonlocaldatabase.h"

setIsSentColumnOnLocalDatabase::setIsSentColumnOnLocalDatabase(piLocalDBWorkerVarSet *VarSet, QState *parent) :
    QState(parent), VarSetPtr(VarSet)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("setIsSentColumnOnLocalDatabase Constructed"));
}

void setIsSentColumnOnLocalDatabase::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Enter setIsSentColumnOnLocalDatabase"));
    if (VarSetPtr->isPendingJsonDataNotPING())
        VarSetPtr->setIsSentColumnOnLocalDatabase();
    qApp->processEvents();
    emit VarSetPtr->directTransitionRequest("updateLocalDatabase");    
}

void setIsSentColumnOnLocalDatabase::onExit(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Leave setIsSentColumnOnLocalDatabase"));
}
