#include "uhvpvicollectordb.h"

UHVPVICollectorDB::UHVPVICollectorDB(bool isUHV2, QObject *parent) : QObject(parent), isAnUHV2(isUHV2)
{
    anIf(UHVPVICollectorDBDbgEn, anTrk("Construct Object"));
    initialize();
}

void UHVPVICollectorDB::initialize()
{
    previousReadState = "emitReadP";
    GS2UHV.Type = QVariant::fromValue(SerialPortWorkerProperty::addAGlobalSignal);
    if (isAnUHV2)
    {
        GS2UHV.DstStrs.append(UHV2WorkerObjName);
        GS2UHV.DstStrs.append(UHV2PVICollectorObjName);
    }
    else
    {
        GS2UHV.DstStrs.append(UHV4WorkerObjName);
        GS2UHV.DstStrs.append(UHV4PVICollectorObjName);
    }
    anIf(UHVPVICollectorDBDbgEn, anAck("OK Initialized !"));
}

bool UHVPVICollectorDB::connectDatabase()
{
    closeDatabaseConnection();
    localDb = QSqlDatabase::cloneDatabase(localQSqlDatabase, this->parent()->objectName());
    if (localDb.open())
    {
        anIf(UHVPVICollectorDBDbgEn, anAck("OK Local Database Connected !"));
        currentGlobalID = 0;
        currentQuery = QSqlQuery(localDb);
    }
    else
    {
        anIf(UHVPVICollectorDBDbgEn, anError("Failed To Connect Local Database !"));
        emit errorOccurred();
        return false;
    }    
    isReady = true;
    return true;
}

bool UHVPVICollectorDB::gotoNextRecord()
{
    anIf(UHVPVICollectorDBDbgEn, anTrk("Retrieve New Record"));
    currentQuery.exec("SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>" +
                        QString::number(currentGlobalID) + " ORDER BY GlobalID ASC LIMIT 1");
    if (!currentQuery.next())
    {
        currentGlobalID = 0;
        currentQuery.exec("SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>" +
                            QString::number(currentGlobalID) + " ORDER BY GlobalID ASC LIMIT 1");
        if (!currentQuery.next())
            return false;
    }
    currentGlobalID = currentQuery.value("GlobalID").toInt();
    currentPNo = currentQuery.value("pumpAddr").toInt();
    currentCH = currentQuery.value("pumpCH").toInt();
    if (isAnUHV2)
    {
        currentBP.SetBPNo(currentPNo).ChannelNo(currentCH);
    }
    else
    {
        currentWP.setWPNo(currentPNo).setChNo(currentCH);
    }
    anIf(UHVPVICollectorDBDbgEn, anVar(currentGlobalID));
    return true;
}

void UHVPVICollectorDB::emitReadP()
{
    if (gotoNextRecord())
    {
        if (isAnUHV2)
        {
            GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentBP.ReadP().GenMsg(),QStringLiteral("")));
        }
        else
        {
            GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentWP.PMeasured().Read().clearDATA().genMSG(),QStringLiteral("")));
        }
        emit Out(GS2UHV);
        emit SignalToUHVEmitted();
    }
    else
    {
        anIf(UHVPVICollectorDBDbgEn, anWarn("Not Found Data!"));
        emit pause();
    }
}

void UHVPVICollectorDB::emitReadV()
{
    if (isAnUHV2)
    {
        GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentBP.ReadV().GenMsg(),QStringLiteral("")));
    }
    else
    {
        GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentWP.VMeasured().Read().clearDATA().genMSG(),QStringLiteral("")));
    }
    emit Out(GS2UHV);
    emit SignalToUHVEmitted();
}

void UHVPVICollectorDB::emitReadI()
{
    if (isAnUHV2)
    {
        GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentBP.ReadI().GenMsg(),QStringLiteral("")));
    }
    else
    {
        GS2UHV.Data = QVariant::fromValue(SerialPortWorkerProperty::DataMessage(currentWP.IMeasured().Read().clearDATA().genMSG(),QStringLiteral("")));
    }
    emit Out(GS2UHV);
    emit SignalToUHVEmitted();
}

void UHVPVICollectorDB::emitMsgToDatabaseUpdatePVI()
{
    anIf(UHVPVICollectorDBDbgEn,
            anAck("Emit PVI To Update Database !");
            anVar(currentGlobalID);
            anVar(currentPressure);
            anVar(currentVoltage);
            anVar(currentCurrent));    
    piLocalDBWorkerVarSet::PVIData currentPVIData;
    currentPVIData.GlobalID = currentGlobalID;
    currentPVIData.Pressure = currentPressure;
    currentPVIData.Voltage = currentVoltage;
    currentPVIData.Current = currentCurrent;
    GlobalSignal updateLocalDatabaseWithPVIData;
    updateLocalDatabaseWithPVIData.Type = QVariant::fromValue(piLocalDBWorkerVarSet::updatePVIData);
    updateLocalDatabaseWithPVIData.Data = QVariant::fromValue(currentPVIData);
    updateLocalDatabaseWithPVIData.DstStrs.append(piLocalDBWorkerObjName);
    emit Out(updateLocalDatabaseWithPVIData);
}

void UHVPVICollectorDB::saveP()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentPressure = "0";
    }
    else
    {
        currentPressure = getDataString();
    }
    anIf(UHVPVICollectorDBDbgEn, anVar(currentPressure));
    emit DataFromUHVObtained();
}

void UHVPVICollectorDB::saveV()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentVoltage = "0";
    }
    else
    {
        currentVoltage = getDataString();
    }
    anIf(UHVPVICollectorDBDbgEn, anVar(currentVoltage));
    emit DataFromUHVObtained();
}

void UHVPVICollectorDB::saveI()
{
    if (currentReplyFromPump.isEmpty())
    {
        currentCurrent = "0";
    }
    else
    {
        currentCurrent = getDataString();
    }
    anIf(UHVPVICollectorDBDbgEn, anVar(currentCurrent));
    emit DataFromUHVObtained();
}

void UHVPVICollectorDB::setPreviousReadState(const QString &StateObjName)
{
    previousReadState = StateObjName;
}

void UHVPVICollectorDB::resume()
{
    emit directTransitionRequest(previousReadState);
}

void UHVPVICollectorDB::processDataFromPump(const QByteArray &data)
{
    anIf(UHVPVICollectorDBDbgEn, anAck("Data From Pump Received !");anVar(data.toHex()););
    currentReplyFromPump = data;
    if (previousReadState=="emitReadP")
        saveP();
    else if (previousReadState=="emitReadV")
        saveV();
    else
        saveI();
}

QString UHVPVICollectorDB::getDataString()
{
    if (isAnUHV2)
    {
        return QString(BinaryProtocol::fromQByteArray(currentReplyFromPump).GetData());
    }
    else
    {
        return QString(WindowProtocol::fromQByteArray(currentReplyFromPump).getDATAMean());
    }
}

void UHVPVICollectorDB::closeDatabaseConnection()
{
    QString currentConnection = localDb.connectionName();
    localDb.close();
    localDb = QSqlDatabase();
    localDb.removeDatabase(currentConnection);
    if (!currentConnection.isEmpty())
    {
        emit DatabaseClosed();
    }
    anIf(UHVPVICollectorDBDbgEn && (!currentConnection.isEmpty()), anAck("Local Database Connection Closed !"));
}

const QMetaEnum UHVPVICollectorDB::DataMetaEnum = QMetaEnum::fromType<UHVPVICollectorDB::Data>();
const QMetaEnum UHVPVICollectorDB::ErrorMetaEnum = QMetaEnum::fromType<UHVPVICollectorDB::Error>();
const QMetaEnum UHVPVICollectorDB::WarningMetaEnum = QMetaEnum::fromType<UHVPVICollectorDB::Warning>();
const QMetaEnum UHVPVICollectorDB::NotificationMetaEnum = QMetaEnum::fromType<UHVPVICollectorDB::Notification>();
