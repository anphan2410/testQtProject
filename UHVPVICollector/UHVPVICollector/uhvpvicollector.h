#ifndef UHVPVICOLLECTOR_H
#define UHVPVICOLLECTOR_H

#define UHVPVICollectorDbgEn 1

#include "uhvpvicollectordb.h"
#include "emitreadp.h"
#include "wait4p.h"
#include "emitreadv.h"
#include "wait4v.h"
#include "emitreadi.h"
#include "wait4i.h"
#include "idleuhvpvicollector.h"
#include "directtransitionforuhvpvicollectorstate.h"

class UHVPVICollector : public QStateMachine
{
    Q_OBJECT
public:
    UHVPVICollector(bool isUHV2, QObject *parent = 0);

signals:
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    UHVPVICollectorDB * currentDb = Q_NULLPTR;
};

#endif // UHVPVICOLLECTOR_H
