#include "smallcoordinator.h"

SmallCoordinator::SmallCoordinator(QObject *parent) : QStateMachine(parent)
{
    CurrentDb = new SmallCoordinatorDB(this);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::ToPiLocalDBWorker, this, &SmallCoordinator::ToPiLocalDBWorker);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::ToUHV2Worker, this, &SmallCoordinator::ToUHV2Worker);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::ToUHV4Worker, this, &SmallCoordinator::ToUHV4Worker);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::ToUHV2PVICollector, this, &SmallCoordinator::ToUHV2PVICollector);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::ToUHV4PVICollector, this, &SmallCoordinator::ToUHV4PVICollector);
    QObject::connect(CurrentDb, &SmallCoordinatorDB::Out, this, &SmallCoordinator::Out);

    QState * main = new QState();
    main->setObjectName("main");

    wait4ReadyWorkers * state1 = new wait4ReadyWorkers(main);
    state1->setObjectName("wait4ReadyWorkers");
    coordinateGlobalSignals * state2 = new coordinateGlobalSignals(CurrentDb,main);
    state2->setObjectName("coordinateGlobalSignals");
    main->setInitialState(state1);

    wait4ErrorHandler4SmallCoordinator * state7 = new wait4ErrorHandler4SmallCoordinator(CurrentDb);
    state7->setObjectName("wait4ErrorHandler4SmallCoordinator");

    state1->addTransition(CurrentDb, &SmallCoordinatorDB::allWorkersReady, state2);
    state2->addTransition(CurrentDb, &SmallCoordinatorDB::firstGlobalSignalAdded, state2);
    state2->addTransition(new directTransitionForSmallCoordinatorState(CurrentDb,state2));
    main->addTransition(CurrentDb, &SmallCoordinatorDB::ErrorOccurred, state7);

    this->addState(main);
    this->addState(state7);
    this->setInitialState(main);

    QObject::connect(state1, &wait4ReadyWorkers::entered, this, &SmallCoordinator::getReady);

    anIf(SmallCoordinatorDbgEn, anTrk("SmallCoordinator Constructed"));
}

void SmallCoordinator::In(const GlobalSignal &aGlobalSignal)
{
    CurrentDb->In(aGlobalSignal);
}
