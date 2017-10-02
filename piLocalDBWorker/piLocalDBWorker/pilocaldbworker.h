#ifndef PILOCALDBWORKER_H
#define PILOCALDBWORKER_H

#define piLocalDBWorkerDbgEn 1

#include <QObject>
#include <QStateMachine>
#include "pilocaldbworkervarset.h"
#include "directtransition4pilocaldbworkerstate.h"
#include "connectdatabase.h"
#include "updatelocaldatabase.h"
#include "updateonlinedatabase.h"
#include "setissentcolumnonlocaldatabase.h"
#include "wait4errorhandler4pilocaldbworker.h"

class piLocalDBWorker : public QStateMachine
{
    Q_OBJECT
public:
    explicit piLocalDBWorker(QObject *parent = nullptr);
signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    piLocalDBWorkerVarSet * currentVarSet = Q_NULLPTR;

};

#endif // PILOCALDBWORKER_H
