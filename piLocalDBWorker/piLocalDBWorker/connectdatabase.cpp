#include "connectdatabase.h"

connectDatabase::connectDatabase(piLocalDBWorkerVarSet *VarSet, QState *parent) :
    QState(parent), VarSetPtr(VarSet)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("connectDatabase Constructed"));
}

void connectDatabase::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Enter connectDatabase"));
    VarSetPtr->connectLocalDatabase();
}

void connectDatabase::onExit(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Leave connectDatabase"));
}
