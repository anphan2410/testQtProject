#include "updateonlinedatabase.h"

updateOnlineDatabase::updateOnlineDatabase(piLocalDBWorkerVarSet *VarSet, QState *parent) :
    QState(parent), VarSetPtr(VarSet)
{
    sendJsonTimer.setParent(this);
    sendJsonTimer.setInterval(7000);
    sendJsonTimer.setSingleShot(true);
    QObject::connect(&sendJsonTimer, &QTimer::timeout, this, [VarSet](){
       anIf(piLocalDBWorkerVarSetDbgEn,
            if (VarSet->isOnline)
            {
                anWarn("Gone Offline");
            }
            else
            {
                anWarn("Still Offline");
            }
            );
       VarSet->isOnline = false;
       emit VarSet->directTransitionRequest("updateLocalDatabase");
    });
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("updateOnlineDatabase Constructed"));
}

void updateOnlineDatabase::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Enter updateOnlineDatabase"));
    if (VarSetPtr->sendPendingJsonDataPackage())
    {
        sendJsonTimer.start();
    }
    else
    {
        emit VarSetPtr->directTransitionRequest("updateLocalDatabase");
    }
    qApp->processEvents();
}

void updateOnlineDatabase::onExit(QEvent *)
{
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("Leave updateOnlineDatabase"));
    sendJsonTimer.stop();
}
