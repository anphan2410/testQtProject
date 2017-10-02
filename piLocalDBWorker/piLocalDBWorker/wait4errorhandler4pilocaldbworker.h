#ifndef WAIT4ERRORHANDLER4PILOCALDBWORKER_H
#define WAIT4ERRORHANDLER4PILOCALDBWORKER_H

#include <QState>
#include <QTimer>
#include "pilocaldbworkervarset.h"

class wait4ErrorHandler4piLocalDBWorker : public QState
{
public:
    wait4ErrorHandler4piLocalDBWorker(piLocalDBWorkerVarSet * VarSet);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer emitErrorTimer;
    piLocalDBWorkerVarSet * VarSetPtr;

};

#endif // WAIT4ERRORHANDLER4PILOCALDBWORKER_H
