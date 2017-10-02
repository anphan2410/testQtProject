#include "smallcoordinatordb.h"

SmallCoordinatorDB::SmallCoordinatorDB(QObject *parent) : QObject(parent)
{
    anIf(SmallCoordinatorDBDbgEn, anTrk("Object Constructed"));
}

void SmallCoordinatorDB::initialize()
{

}

void SmallCoordinatorDB::dispose()
{
    clearPrioritizedBuffer();
    clearError();
}

void SmallCoordinatorDB::setError(const SmallCoordinatorDB::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(SmallCoordinatorDBDbgEn, anWarn("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void SmallCoordinatorDB::clearError()
{
    anIf(SmallCoordinatorDBDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void SmallCoordinatorDB::addOneGlobalSignal(const GlobalSignal &aGlobalSignal)
{
    if (prioritizedBuffer.contains(aGlobalSignal.Priority))
    {
        prioritizedBuffer[aGlobalSignal.Priority].append(aGlobalSignal);
    }
    else
    {
        prioritizedBuffer.insert(aGlobalSignal.Priority, QList<GlobalSignal>({aGlobalSignal}));
    }
    if ((prioritizedBuffer.size() == 1) && (prioritizedBuffer.first().size() == 1))
    {
        emit firstGlobalSignalAdded();
    }
    else
    {
        emit oneGlobalSignalAdded();
    }
}

GlobalSignal &SmallCoordinatorDB::takeOutFirstOfMostPrioritizedGlobalSignals()
{
    GlobalSignal &tmpReturn = prioritizedBuffer.last().first();
    prioritizedBuffer.last().removeFirst();
    clearEmptyList();
    return tmpReturn;
}

GlobalSignal &SmallCoordinatorDB::takeOutLastOfMostPrioritizedGlobalSignals()
{
    GlobalSignal &tmpReturn = prioritizedBuffer.last().last();
    prioritizedBuffer.last().removeLast();
    clearEmptyList();
    return tmpReturn;
}

void SmallCoordinatorDB::clearEmptyList()
{
    while (prioritizedBuffer.size())
    {
        if (prioritizedBuffer.last().size())
            break;
        else
        {
            prioritizedBuffer.remove(prioritizedBuffer.lastKey());
        }
    }
}

void SmallCoordinatorDB::clearCache()
{
    currentGlobalSignal.Data.clear();
    currentGlobalSignal.Type.clear();
    currentGlobalSignal.DstStrs.clear();
    currentGlobalSignal.Key.clear();
    currentGlobalSignal.Priority = 0;
    currentGlobalSignal.SignalPriority = 0;
}

void SmallCoordinatorDB::clearPrioritizedBuffer()
{
    if (prioritizedBuffer.size())
        prioritizedBuffer.clear();
}

void SmallCoordinatorDB::checkIfAllWorkersReady()
{
    if (isPiLocalDBWorkerReady && isUHV2WorkerReady && isUHV4WorkerReady && isUHV2PVICollectorReady & isUHV4PVICollectorReady)
    {
        anIf(SmallCoordinatorDBDbgEn, anAck("All Workers Is Ready !"));
        isAllWorkersReady = true;
        emit allWorkersReady();
    }
}

void SmallCoordinatorDB::In(const GlobalSignal &aGlobalSignal)
{
    if (!isAllWorkersReady)
    {
        QString aGlobalSignalTypeTypeName = aGlobalSignal.Type.typeName();
        int aGlobalSignalTypeToInt = aGlobalSignal.Type.toInt();
        if ((aGlobalSignalTypeTypeName==QStringLiteral("SerialPortWorkerProperty::Data"))
                && (aGlobalSignalTypeToInt==SerialPortWorkerProperty::requestPortName))
        {
            QString SenderName = aGlobalSignal.Data.toString();
            GlobalSignal replyUHVPortName;
            replyUHVPortName.Type = QVariant::fromValue(SerialPortWorkerProperty::replyPortName);
            if (SenderName == UHV2WorkerObjName)
            {
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("COM3"));
                emit ToUHV2Worker(replyUHVPortName);
            }
            else if (SenderName == UHV4WorkerObjName)
            {
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("COM5"));
                emit ToUHV4Worker(replyUHVPortName);
            }
        }
        else
        {
            if ((aGlobalSignalTypeTypeName == QStringLiteral("piLocalDBWorkerVarSet::Notification"))
                    && (aGlobalSignalTypeToInt == piLocalDBWorkerVarSet::readyToWork))
            {
                anIf(SmallCoordinatorDBDbgEn, anAck("piLocalDBWorkerVarSet::readyToWork"));
                if (aGlobalSignal.Data.toString() == piLocalDBWorkerObjName)
                {
                    isPiLocalDBWorkerReady = true;
                    anIf(SmallCoordinatorDBDbgEn, anVar(isPiLocalDBWorkerReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerProperty::Notification"))
                        && (aGlobalSignalTypeToInt == SerialPortWorkerProperty::readyToWork))
            {
                anIf(SmallCoordinatorDBDbgEn, anAck("SerialPortWorkerProperty::readyToWork"));
                QString WhoIsReadyToWork = aGlobalSignal.Data.toString();
                if (WhoIsReadyToWork == UHV2WorkerObjName)
                {
                    isUHV2WorkerReady = true;
                    anIf(SmallCoordinatorDBDbgEn, anVar(isUHV2WorkerReady));
                }
                else if (WhoIsReadyToWork == UHV4WorkerObjName)
                {
                    isUHV4WorkerReady = true;
                    anIf(SmallCoordinatorDBDbgEn, anVar(isUHV4WorkerReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("UHVPVICollectorDB::Notification"))
                        && (aGlobalSignalTypeToInt == UHVPVICollectorDB::readyToWork))
            {
                anIf(SmallCoordinatorDBDbgEn, anAck("UHVPVICollectorDB::readyToWork"));
                QString WhoIsReadyToWork = aGlobalSignal.Data.toString();
                if (WhoIsReadyToWork == UHV2PVICollectorObjName)
                {
                    isUHV2PVICollectorReady = true;
                    anIf(SmallCoordinatorDBDbgEn, anVar(isUHV2PVICollectorReady));
                }
                else if (WhoIsReadyToWork == UHV4PVICollectorObjName)
                {
                    isUHV4PVICollectorReady = true;
                    anIf(SmallCoordinatorDBDbgEn, anVar(isUHV4PVICollectorReady));
                }
            }
            checkIfAllWorkersReady();
        }
    }
    else
    {
        addOneGlobalSignal(aGlobalSignal);
    }
}

void SmallCoordinatorDB::executeGlobalSignals()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = prioritizedBuffer.last().first();
        if (currentGlobalSignal.DstStrs.size())
        {
            currentDestination = currentGlobalSignal.DstStrs.takeFirst();
            if (currentDestination == piLocalDBWorkerObjName)
            {
                emit ToPiLocalDBWorker(currentGlobalSignal);
            }
            else if (currentDestination == UHV2WorkerObjName)
            {
                emit ToUHV2Worker(currentGlobalSignal);
            }
            else if (currentDestination == UHV4WorkerObjName)
            {
                emit ToUHV4Worker(currentGlobalSignal);
            }
            else if (currentDestination == UHV2PVICollectorObjName)
            {
                emit ToUHV2PVICollector(currentGlobalSignal);
            }
            else if (currentDestination == UHV4PVICollectorObjName)
            {
                emit ToUHV4PVICollector(currentGlobalSignal);
            }
            else if (currentDestination == SmallCoordinatorObjName)
            {
                if (currentGlobalSignal.Type.typeName() == QStringLiteral("SmallCoordinatorDB::Data"))
                {
                    switch (currentGlobalSignal.Type.toInt()) {
                    case SmallCoordinatorDB::resumeAllCollectors:
                    {
                        GlobalSignal resumeUHVPVICollector;
                        resumeUHVPVICollector.Type = QVariant::fromValue(UHVPVICollectorDB::resumeMachine);
                        emit ToUHV2PVICollector(resumeUHVPVICollector);
                        emit ToUHV4PVICollector(resumeUHVPVICollector);
                        break;
                    }                    
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void SmallCoordinatorDB::emitErrorGlobalSignal()
{
    anIf(SmallCoordinatorDBDbgEn,
         anError("Emit SmallCoordinatorDB::Error");
         anInfo("ErrorType: " + QString(SmallCoordinatorDB::ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    emit Out(errorGlobalSignal);
}

const QMetaEnum SmallCoordinatorDB::DataMetaEnum = QMetaEnum::fromType<SmallCoordinatorDB::Data>();
const QMetaEnum SmallCoordinatorDB::ErrorMetaEnum = QMetaEnum::fromType<SmallCoordinatorDB::Error>();
const QMetaEnum SmallCoordinatorDB::WarningMetaEnum = QMetaEnum::fromType<SmallCoordinatorDB::Warning>();
const QMetaEnum SmallCoordinatorDB::NotificationMetaEnum = QMetaEnum::fromType<SmallCoordinatorDB::Notification>();
