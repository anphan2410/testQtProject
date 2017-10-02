#include "uhvpvicollector.h"

UHVPVICollector::UHVPVICollector(bool isUHV2, QObject *parent) : QStateMachine(parent)
{    
    currentDb = new UHVPVICollectorDB(isUHV2,this);
    QObject::connect(currentDb, &UHVPVICollectorDB::Out, this, &UHVPVICollector::Out);

    emitReadP * state1 = new emitReadP(currentDb);
    state1->setObjectName("emitReadP");
    wait4P * state2 = new wait4P(currentDb);
    state2->setObjectName("wait4P");
    emitReadV * state3 = new emitReadV(currentDb);
    state3->setObjectName("emitReadV");
    wait4V * state4 = new wait4V(currentDb);
    state4->setObjectName("wait4V");
    emitReadI * state5 = new emitReadI(currentDb);
    state5->setObjectName("emitReadI");
    wait4I * state6 = new wait4I(currentDb);
    state6->setObjectName("wait4I");
    idleUHVPVICollector * state7 = new idleUHVPVICollector(currentDb);
    state7->setObjectName("idleUHVPVICollector");

    state1->addTransition(currentDb, &UHVPVICollectorDB::SignalToUHVEmitted, state2);
    state2->addTransition(currentDb, &UHVPVICollectorDB::DataFromUHVObtained, state3);
    state3->addTransition(currentDb, &UHVPVICollectorDB::SignalToUHVEmitted, state4);
    state4->addTransition(currentDb, &UHVPVICollectorDB::DataFromUHVObtained, state5);
    state5->addTransition(currentDb, &UHVPVICollectorDB::SignalToUHVEmitted, state6);
    state6->addTransition(currentDb, &UHVPVICollectorDB::DataFromUHVObtained, state1);

    state1->addTransition(currentDb, &UHVPVICollectorDB::pause, state7);
    state3->addTransition(currentDb, &UHVPVICollectorDB::pause, state7);
    state5->addTransition(currentDb, &UHVPVICollectorDB::pause, state7);
    state7->addTransition(new directTransitionForUHVPVICollectorState(currentDb, state1));
    state7->addTransition(new directTransitionForUHVPVICollectorState(currentDb, state3));
    state7->addTransition(new directTransitionForUHVPVICollectorState(currentDb, state5));

    this->addState(state1);
    this->addState(state2);
    this->addState(state3);
    this->addState(state4);
    this->addState(state5);
    this->addState(state6);
    this->addState(state7);
    this->setInitialState(state7);
    this->setErrorState(state7);

    anIf(UHVPVICollectorDbgEn, anTrk("UHVPVICollector Constructed"));

}

void UHVPVICollector::In(const GlobalSignal &aGlobalSignal)
{
    anIf(UHVPVICollectorDBDbgEn, anTrk("UHVPVICollector Receives A Signal"));
    QString enumVarTypeName(aGlobalSignal.Type.typeName());
    if (enumVarTypeName == "UHVPVICollectorDB::Data")
    {
        switch (aGlobalSignal.Type.toInt()) {
        case UHVPVICollectorDB::pauseMachine:
        {
            anIf(UHVPVICollectorDbgEn, anTrk("pauseMachine"));
            emit currentDb->pause();
            break;
        }
        case UHVPVICollectorDB::resumeMachine:
        {
            anIf(UHVPVICollectorDbgEn, anTrk("resumeMachine"));
            currentDb->resume();
            break;
        }
        default:
            break;
        }
    }
    else if (enumVarTypeName == "SerialPortWorkerProperty::Data")
    {
        switch (aGlobalSignal.Type.toInt()) {
        case SerialPortWorkerProperty::replyAGlobalSignal:
        {
            anIf(UHVPVICollectorDbgEn, anTrk("SerialPortWorkerProperty::replyAGlobalSignal"));
            currentDb->processDataFromPump(aGlobalSignal.Data.value<SerialPortWorkerProperty::DataMessage>().first);
            break;
        }
        default:
            break;
        }
    }
}
