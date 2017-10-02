#include "pilocaldbworker.h"

piLocalDBWorker::piLocalDBWorker(QObject *parent) : QStateMachine(parent)
{
    currentVarSet = new piLocalDBWorkerVarSet(this);
    QObject::connect(currentVarSet, &piLocalDBWorkerVarSet::Out, this, &piLocalDBWorker::Out);

    QState * main = new QState();
    main->setObjectName("main");

    connectDatabase * state1 = new connectDatabase(currentVarSet,main);
    state1->setObjectName("connectDatabase");
    updateLocalDatabase * state2 = new updateLocalDatabase(currentVarSet,main);
    state2->setObjectName("updateLocalDatabase");
    updateOnlineDatabase * state3 = new updateOnlineDatabase(currentVarSet,main);
    state3->setObjectName("updateOnlineDatabase");
    setIsSentColumnOnLocalDatabase * state4 = new setIsSentColumnOnLocalDatabase(currentVarSet,main);
    state4->setObjectName("setIsSentColumnOnLocalDatabase");

    state1->addTransition(currentVarSet, &piLocalDBWorkerVarSet::DatabaseConnected, state2);
    state2->addTransition(currentVarSet, &piLocalDBWorkerVarSet::firstGlobalSignalAdded, state2);
    state2->addTransition(new directTransition4piLocalDBWorkerState(currentVarSet,state3));
    state3->addTransition(new directTransition4piLocalDBWorkerState(currentVarSet,state2));
    state3->addTransition(currentVarSet, &piLocalDBWorkerVarSet::jsonPackageTransmitted, state4);
    state4->addTransition(new directTransition4piLocalDBWorkerState(currentVarSet,state2));

    wait4ErrorHandler4piLocalDBWorker * state7 = new wait4ErrorHandler4piLocalDBWorker(currentVarSet);
    state7->setObjectName("wait4ErrorHandler4piLocalDBWorker");

    main->setInitialState(state1);
    main->addTransition(currentVarSet, &piLocalDBWorkerVarSet::ErrorOccurred, state7);

    this->addState(main);
    this->addState(state7);
    this->setInitialState(main);

    anIf(piLocalDBWorkerDbgEn, anTrk("piLocalDBWorker Constructed"));
}

void piLocalDBWorker::In(const GlobalSignal &aGlobalSignal)
{
    if (aGlobalSignal.Type.typeName() == QStringLiteral("piLocalDBWorkerVarSet::Data"))
    {
        currentVarSet->addOneGlobalSignal(aGlobalSignal);
    }
}
