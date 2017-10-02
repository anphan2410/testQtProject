#ifndef SMALLCOORDINATOR_H
#define SMALLCOORDINATOR_H

#define SmallCoordinatorDbgEn 1

#include <QStateMachine>
#include "smallcoordinatordb.h"
#include "directtransitionforsmallcoordinatorstate.h"
#include "wait4readyworkers.h"
#include "coordinateglobalsignals.h"
#include "wait4errorhandler4smallcoordinator.h"

class SmallCoordinator : public QStateMachine
{
    Q_OBJECT
public:
    explicit SmallCoordinator(QObject *parent = nullptr);
signals:
    void ToUHV2Worker(const GlobalSignal &);
    void ToUHV4Worker(const GlobalSignal &);
    void ToUHV2PVICollector(const GlobalSignal &);
    void ToUHV4PVICollector(const GlobalSignal &);
    void ToPiLocalDBWorker(const GlobalSignal &);
    void getReady();
    void Out(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    SmallCoordinatorDB * CurrentDb = Q_NULLPTR;
};

#endif // SMALLCOORDINATOR_H
