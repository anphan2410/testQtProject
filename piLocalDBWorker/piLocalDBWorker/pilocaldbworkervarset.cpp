#include "pilocaldbworkervarset.h"

piLocalDBWorkerVarSet::piLocalDBWorkerVarSet(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<piLocalDBWorkerVarSet::PVIData>("piLocalDBWorkerVarSet::PVIData");
    qRegisterMetaType<piLocalDBWorkerVarSet::RFIDData>("piLocalDBWorkerVarSet::RFIDData");
    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("piLocalDBWorkerVarSet Constructed"));
}

void piLocalDBWorkerVarSet::initialize()
{
    dispose();
}

void piLocalDBWorkerVarSet::dispose()
{
    if (currentGlobalSignal)
    {
        delete currentGlobalSignal;
        currentGlobalSignal = Q_NULLPTR;
    }
}

bool piLocalDBWorkerVarSet::connectLocalDatabase()
{
    closeLocalDatabaseConnection();
    localDb = QSqlDatabase::cloneDatabase(localQSqlDatabase, this->parent()->objectName());
    if (localDb.open())
    {
        anIf(piLocalDBWorkerVarSetDbgEn, anAck("OK Local Database Connected !"));
        sourceQuery = QSqlQuery(localDb);
        tmpQuery = QSqlQuery(localDb);
        tmpQuery2 = QSqlQuery(localDb);
        instantQuery = QSqlQuery(localDb);
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(piLocalDBWorkerVarSet::readyToWork);
        iamReady.Data = QVariant::fromValue(this->parent()->objectName());
        iamReady.DstStrs.append(SmallCoordinatorObjName);
        iamReady.SignalPriority = 100;
        emit Out(iamReady);
        emit DatabaseConnected();
        return true;
    }
    else
    {
        anIf(piLocalDBWorkerVarSetDbgEn, anError("Failed To Connect Local Database !"));
        setError(piLocalDBWorkerVarSet::QSqlError,localDb.lastError().text());
    }
    return false;
}

void piLocalDBWorkerVarSet::closeLocalDatabaseConnection()
{
    QString currentConnection = localDb.connectionName();
    localDb.close();
    localDb = QSqlDatabase();
    localDb.removeDatabase(currentConnection);
    if (!currentConnection.isEmpty())
    {
        emit DatabaseClosed();
    }
    anIf(piLocalDBWorkerVarSetDbgEn && (!currentConnection.isEmpty()), anAck("Local Database Connection Closed !"));
}

void piLocalDBWorkerVarSet::setError(const piLocalDBWorkerVarSet::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(piLocalDBWorkerVarSetDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void piLocalDBWorkerVarSet::clearError()
{
    anIf(piLocalDBWorkerVarSetDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void piLocalDBWorkerVarSet::addOneGlobalSignal(const GlobalSignal &aGlobalSignal)
{
    if (prioritizedBuffer.contains(aGlobalSignal.Priority))
    {
        prioritizedBuffer.value(aGlobalSignal.Priority)->append(aGlobalSignal);
    }
    else
    {
        prioritizedBuffer.insert(aGlobalSignal.Priority, new QList<GlobalSignal>({aGlobalSignal}));
    }
    if ((prioritizedBuffer.size() == 1) && (prioritizedBuffer.first()->size() == 1))
    {
        emit firstGlobalSignalAdded();
    }
    else
    {
        emit oneGlobalSignalAdded();
    }
}

void piLocalDBWorkerVarSet::requestControlTableCheck()
{
    instantQuery.exec("SELECT COUNT(*) AS TotalControlRequests FROM control");
    instantQuery.first();
    for (int i = instantQuery.value("TotalControlRequests").toInt(); i>0; --i)
    {
        addOneGlobalSignal(constGlobalSignalCheckControlTable);
    }
}

GlobalSignal &piLocalDBWorkerVarSet::takeOutFirstOfMostPrioritizedGlobalSignals()
{
    GlobalSignal &tmpReturn = prioritizedBuffer.last()->first();
    prioritizedBuffer.last()->removeFirst();
    clearEmptyList();
    return tmpReturn;
}

GlobalSignal &piLocalDBWorkerVarSet::takeOutLastOfMostPrioritizedGlobalSignals()
{
    GlobalSignal &tmpReturn = prioritizedBuffer.last()->last();
    prioritizedBuffer.last()->removeLast();
    clearEmptyList();
    return tmpReturn;
}

void piLocalDBWorkerVarSet::clearEmptyList()
{
    while (prioritizedBuffer.size())
    {
        if (prioritizedBuffer.last()->size())
            break;
        else
        {
            delete prioritizedBuffer.last();
            prioritizedBuffer.last() = Q_NULLPTR;
            prioritizedBuffer.remove(prioritizedBuffer.lastKey());
        }
    }
}

void piLocalDBWorkerVarSet::clearAllReservedStrings()
{
    reservedStr1.clear();
    reservedStr2.clear();
}

bool piLocalDBWorkerVarSet::isPendingJsonDataNotPING()
{
    if (pendingJSonData == constPINGJsonPackage)
        return false;
    else
        return true;
}

void piLocalDBWorkerVarSet::setIsSentColumnOnLocalDatabase()
{
    if (isOnline)
    {
        switch (currentGlobalSignal->Type.toInt()) {
        case checkControlTable:
        {
            if (reservedStr1.size() && reservedStr2.size())
                sourceQuery.exec("UPDATE " + reservedStr1 + " SET isSent=1 WHERE Time='" + reservedStr2 + "'");
            break;
        }
        case updatePVIData:
        {
            if (reservedStr1.size() && reservedStr2.size())
                sourceQuery.exec("UPDATE " + reservedStr1 + " SET isSent=1 WHERE Time='" + reservedStr2 + "'");
            break;
        }
        case updateRFIDData:
        {
            if (reservedStr1.size() && reservedStr2.size())
                sourceQuery.exec("UPDATE " + reservedStr1 + " SET isSent=1 WHERE Time='" + reservedStr2 + "'");
            break;
        }
        case syncOfflineData:
        {
            sourceQuery.exec("SELECT RFID FROM stations");
            while (sourceQuery.next())
            {
                tmpQuery.exec("UPDATE " + sourceQuery.value("RFID").toString().toLower() + " SET isSent=1");
            }
            anIf(piLocalDBWorkerVarSetDbgEn, anAck("Offline Data Synchronized !"));
            break;
        }
        default:
            break;
        }
    }
    clearAllReservedStrings();
}

void piLocalDBWorkerVarSet::emitErrorGlobalSignal()
{
    anIf(piLocalDBWorkerVarSetDbgEn,
         anError("Emit piLocalDBWorkerVarSet::Error");
         anInfo("ErrorType: " + QString(piLocalDBWorkerVarSet::ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 124;
    emit Out(errorGlobalSignal);
}

void piLocalDBWorkerVarSet::sendJsonDataPackage(const QByteArray &data)
{
    if (networkReply)
        delete networkReply;
    networkReply = NAManager.post(onlineDb, data);
    QObject::connect(networkReply, &QNetworkReply::finished, this, [&](){
        if (!isOnline)
        {
            GlobalSignal tmpGlobalSignal;
            tmpGlobalSignal.Type = QVariant::fromValue(piLocalDBWorkerVarSet::syncOfflineData);
            tmpGlobalSignal.Priority = 111;
            addOneGlobalSignal(tmpGlobalSignal);
            anIf(piLocalDBWorkerVarSetDbgEn, anAck("Offline Data Sync Requested !"))
        }
        isOnline = true;
        anIf(piLocalDBWorkerVarSetDbgEn, anAck("Json Package Transmitted !"));
        emit jsonPackageTransmitted();
    }, uniqueQtConnectionType);
    anIf(piLocalDBWorkerVarSetDbgEn, anInfo("Try Sending Json Package ..." << data));
}

bool piLocalDBWorkerVarSet::sendPendingJsonDataPackage()
{
    if (pendingJSonData.size())
    {
        sendJsonDataPackage(pendingJSonData);
        if (isPendingJsonDataNotPING())
            pendingJSonData.clear();
        return true;
    }
    return false;
}

const QJsonObject piLocalDBWorkerVarSet::createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time)
{
    QJsonObject value
    {
        {"hv",HV,},
        {"valve",Valve,},
        {"pressure",Pressure,},
        {"current",Current,},
        {"voltage",Voltage,},
        {"rfid",dstRFID,},
        {"time",Time,}
    };
    QJsonObject pump
    {
        {GlobalID,value},
    };
    return pump;
}

const QJsonArray piLocalDBWorkerVarSet::getPendingJsonDataPackage()
{
    QSqlQuery tmpScopedSourceQuerry(localDb);
    QSqlQuery tmpScopedQuerry(localDb);
    QJsonArray tmpReturn;
    QString tmpScopedString;
    tmpScopedSourceQuerry.exec("SELECT RFID, setRoughValveOn, setHVOn, GlobalID FROM stations");
    while (tmpScopedSourceQuerry.next())
    {
        tmpScopedString = tmpScopedSourceQuerry.value("RFID").toString();
        tmpScopedQuerry.exec("SELECT * FROM " + tmpScopedString.toLower() + "  WHERE isSent<>1 ORDER BY Time ASC");
        while (tmpScopedQuerry.next())
        {
            tmpReturn << createAJsonDataPackage(tmpScopedSourceQuerry.value("GlobalID").toString(),
                                          tmpScopedSourceQuerry.value("setHVOn").toString(),
                                          tmpScopedSourceQuerry.value("setRoughValveOn").toString(),
                                          tmpScopedQuerry.value("Pressure").toString(),
                                          tmpScopedQuerry.value("Current").toString(),
                                          tmpScopedQuerry.value("Voltage").toString(),
                                          tmpScopedString,
                                          tmpScopedQuerry.value("Time").toString());
        }
    }
    return tmpReturn;
}

void piLocalDBWorkerVarSet::updateLocalDatabase()
{
    pendingJSonData.clear();
    clearAllReservedStrings();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = &(prioritizedBuffer.last()->first());
        switch (currentGlobalSignal->Type.toInt()) {
        case checkControlTable:
        {
            anIf(piLocalDBWorkerVarSetDbgEn, anTrk("checkControlTable"));
            sourceQuery.exec("SELECT GlobalID,HV,Valve FROM control WHERE (GlobalID Is Not Null And GlobalID>0) ORDER BY Time ASC LIMIT 1");
            if (sourceQuery.next())
            {                
                tmpQuery.exec("SELECT pumpType,pumpAddr,pumpCH,sdcsAddr,sdcsCH,RFID FROM stations WHERE GlobalID=" + sourceQuery.value("GlobalID").toString() + " LIMIT 1");
                anIf(piLocalDBWorkerVarSetDbgEn, anTrk("SQL: " << "SELECT pumpType,pumpAddr,pumpCH,sdcsAddr,sdcsCH,RFID FROM stations WHERE GlobalID=" + sourceQuery.value("GlobalID").toString() + " LIMIT 1"));
                if (tmpQuery.next())
                {
                    anIf(piLocalDBWorkerVarSetDbgEn, anInfo("OK Record Found"));
                    if (tmpQuery.value("pumpType").toInt()==0)
                    {
                        tmpBP.SetBPNo(tmpQuery.value("pumpAddr").toInt());
                        tmpBP.Ch(tmpQuery.value("pumpCH").toInt()).HVSwitch();
                        if (sourceQuery.value("HV").toInt()==0)
                        {
                            tmpBP.Off();
                        }
                        else
                        {
                            tmpBP.On();
                        }
                        tmpBP.GenMsg();
                        anIf(piLocalDBWorkerVarSetDbgEn, anAck("UHV2: " << tmpBP.GetMessageTranslation()));
                        emit controlUHV2(tmpBP);
                    }
                    else if (tmpQuery.value("pumpType").toInt()==1)
                    {
                        tmpWP.setWPNo(tmpQuery.value("pumpAddr").toInt());
                        tmpWP.HVOnOff().setChNo(tmpQuery.value("pumpCH").toInt()).Write();
                        if (sourceQuery.value("HV").toInt()==0)
                        {
                            tmpWP.setOFF();
                        }
                        else
                        {
                            tmpWP.setON();
                        }
                        tmpWP.genMSG();
                        anIf(piLocalDBWorkerVarSetDbgEn, anAck("UHV4: " << tmpWP.getMSG().toHex()));
                        emit controlUHV4(tmpWP);
                    }
                    else
                    {
                        anIf(piLocalDBWorkerVarSetDbgEn, anWarn("Invalid pumpType at GlobalID=" + sourceQuery.value("GlobalID").toString()));
                    }
                    tmpCP.setSdcsId(tmpQuery.value("sdcsAddr").toInt()).setChId(tmpQuery.value("sdcsCH").toInt()).setRFID(QByteArray::fromHex(tmpQuery.value("RFID").toByteArray().mid(1)));
                    if (sourceQuery.value("Valve").toInt()==0)
                    {
                        tmpCP.setValveOff();
                    }
                    else
                    {
                        tmpCP.setValveOn();
                    }
                    anIf(piLocalDBWorkerVarSetDbgEn, anAck("CAN: " << tmpCP.getMsgStr()));
                    emit controlValve(tmpCP);
                    tmpQuery.exec("UPDATE stations SET setRoughValveOn=" +
                                  sourceQuery.value("Valve").toString() +
                                  ",setHVOn=" + sourceQuery.value("HV").toString() +
                                  ",time=NOW() WHERE GlobalID=" + sourceQuery.value("GlobalID").toString());
                    anIf(piLocalDBWorkerVarSetDbgEn, anTrk("SQL: " << "UPDATE stations SET setRoughValveOn=" +
                                                           sourceQuery.value("Valve").toString() +
                                                           ",setHVOn=" + sourceQuery.value("HV").toString() +
                                                           ",time=NOW() WHERE GlobalID=" + sourceQuery.value("GlobalID").toString()));
                    if (isOnline)
                    {
                        tmpQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + sourceQuery.value("GlobalID").toString() + " LIMIT 1");
                        tmpQuery.next();
                        tmpStr = tmpQuery.value("RFID").toString();
                        tmpQuery2.exec("SELECT * FROM " + tmpStr.toLower() + " ORDER BY Time DESC LIMIT 1");
                        tmpQuery2.next();
                        tmpStr5 = tmpQuery2.value("Time").toString();
                        pendingJSonData = QJsonDocument(createAJsonDataPackage(sourceQuery.value("GlobalID").toString(),
                                                                         tmpQuery.value("setHVOn").toString(),
                                                                         tmpQuery.value("setRoughValveOn").toString(),
                                                                         tmpQuery2.value("Pressure").toString(),
                                                                         tmpQuery2.value("Voltage").toString(),
                                                                         tmpQuery2.value("Current").toString(),
                                                                         tmpStr,
                                                                         tmpStr5)).toJson(QJsonDocument::Compact);
                        reservedStr1 = tmpStr.toLower();
                        reservedStr2 = tmpStr5;
                    }
                    else
                    {
                        pendingJSonData = constPINGJsonPackage;
                    }
                }                
                tmpQuery.exec("DELETE FROM control WHERE (GlobalID=" + sourceQuery.value("GlobalID").toString() +
                                                    " And HV=" + sourceQuery.value("HV").toString() +
                                                    " And Valve=" + sourceQuery.value("Valve").toString() + ")");
                anIf(piLocalDBWorkerVarSetDbgEn, anTrk("SQL: " << "DELETE FROM control WHERE (GlobalID=" + sourceQuery.value("GlobalID").toString() +
                                                       " And HV=" + sourceQuery.value("HV").toString() +
                                                       " And Valve=" + sourceQuery.value("Valve").toString() + ")"));
            }            
            break;
        }
        case updatePVIData:
        {
            anIf(piLocalDBWorkerVarSetDbgEn, anTrk("updatePVIData"));
            PVIData currentPVIData = currentGlobalSignal->Data.value<piLocalDBWorkerVarSet::PVIData>();
            tmpStr1 = QString::number(currentPVIData.GlobalID);
            tmpStr2 = currentPVIData.Pressure;
            tmpStr3 = currentPVIData.Voltage;
            tmpStr4 = currentPVIData.Current;
            sourceQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + tmpStr1 + " LIMIT 1");
            if (sourceQuery.next())
            {
                tmpStr = sourceQuery.value("RFID").toString();
                tmpQuery.prepare("INSERT INTO " + tmpStr.toLower() + " (Time,Pressure,Voltage,Current) "
                                    "VALUES (NOW()," + tmpStr2 + "," + tmpStr3 + "," + tmpStr4 + ")");
                if (tmpQuery.exec())
                {
                    if (isOnline)
                    {
                        tmpQuery2.exec("SELECT * FROM " + tmpStr.toLower() + " ORDER BY Time DESC LIMIT 1");
                        tmpQuery2.next();
                        tmpStr5 = tmpQuery2.value("Time").toString();
                        pendingJSonData = QJsonDocument(createAJsonDataPackage(tmpStr1,
                                                                       sourceQuery.value("setHVOn").toString(),
                                                                       sourceQuery.value("setRoughValveOn").toString(),
                                                                       tmpStr2,
                                                                       tmpStr3,
                                                                       tmpStr4,
                                                                       tmpStr,
                                                                       tmpStr5)).toJson(QJsonDocument::Compact);
                        reservedStr1 = tmpStr.toLower();
                        reservedStr2 = tmpStr5;
                    }
                    else
                    {
                        pendingJSonData = constPINGJsonPackage;
                    }
                }
                else
                {
                    anIf(piLocalDBWorkerVarSetDbgEn, anWarn("Not Found Table " + tmpStr.toLower()));
                }
            }
            break;
        }
        case updateRFIDData:
        {
            anIf(piLocalDBWorkerVarSetDbgEn, anTrk("updateRFIDData"));
            RFIDData currentRFIDData = currentGlobalSignal->Data.value<piLocalDBWorkerVarSet::RFIDData>();
            quint32 GlobalID = currentRFIDData.GlobalID;
            QString srcRFID = currentRFIDData.srcRFID;
            tmpStr = QString::number(GlobalID);
            tmpStr1 = "R" + srcRFID;
            sourceQuery.exec("SELECT setHVOn,setRoughValveOn,RFID FROM stations WHERE GlobalID=" + tmpStr);
            if (sourceQuery.next())
            {
                if (sourceQuery.value("RFID").toString()!=tmpStr1)
                {
                    anIf(piLocalDBWorkerVarSetDbgEn, anAck("RFID Changed At GlobalID=" + tmpStr));
                    tmpQuery.exec("UPDATE stations SET RFID='" + tmpStr1 + "',time=NOW() WHERE GlobalID=" + tmpStr);
                    tmpQuery.exec("SELECT * FROM rfids WHERE RFID='" + tmpStr1 + "'");
                    if (!tmpQuery.next())
                    {
                        anIf(piLocalDBWorkerVarSetDbgEn, anInfo("New RFID"));
                        if (tmpQuery.exec("INSERT INTO rfids (RFID) VALUES ('" + tmpStr1 + "')"))
                        {
                            tmpQuery.exec("CREATE TABLE IF NOT EXISTS " + tmpStr1.toLower() + " (Time DATETIME NOT NULL,Pressure DOUBLE NULL,Voltage DOUBLE NULL,Current DOUBLE NULL,`isSent` tinyint(1) NOT NULL DEFAULT '0',PRIMARY KEY (Time),UNIQUE INDEX Time_UNIQUE (Time ASC)) ENGINE = InnoDB");
                            anIf(piLocalDBWorkerVarSetDbgEn, anAck("New RFID Inserted Into rfids Table"));
                            if (isOnline)
                            {
                                tmpQuery2.exec("SELECT * FROM " + tmpStr1.toLower() + " ORDER BY Time DESC LIMIT 1");
                                if (tmpQuery2.next())
                                {
                                    tmpStr2 = tmpQuery2.value("Pressure").toString();
                                    tmpStr3 = tmpQuery2.value("Voltage").toString();
                                    tmpStr4 = tmpQuery2.value("Current").toString();
                                    tmpStr5 = tmpQuery2.value("Time").toString();
                                }
                                else
                                {
                                    tmpStr2.clear();
                                    tmpStr3.clear();
                                    tmpStr4.clear();
                                    tmpStr5.clear();
                                }
                                pendingJSonData = QJsonDocument(createAJsonDataPackage(tmpStr,
                                                                               sourceQuery.value("setHVOn").toString(),
                                                                               sourceQuery.value("setRoughValveOn").toString(),
                                                                               tmpStr2,
                                                                               tmpStr3,
                                                                               tmpStr4,
                                                                               tmpStr1,
                                                                               tmpStr5)).toJson(QJsonDocument::Compact);
                                reservedStr1 = tmpStr1.toLower();
                                reservedStr2 = tmpStr5;
                            }
                            else
                            {
                                pendingJSonData = constPINGJsonPackage;
                            }
                        }
                    }
                }
            }
            break;
        }
        case syncOfflineData:
        {
            anIf(piLocalDBWorkerVarSetDbgEn, anTrk("syncOfflineData"));
            if (isOnline)
                pendingJSonData = QJsonDocument(getPendingJsonDataPackage()).toJson(QJsonDocument::Compact);
            else
            {
                anIf(piLocalDBWorkerVarSetDbgEn, anWarn("Offline Again !"));
                pendingJSonData = constPINGJsonPackage;
            }
            break;
        }
        default:
            break;
        }
    }
}

const QNetworkRequest piLocalDBWorkerVarSet::onlineDb = [](){
    QNetworkRequest tmp(QUrl("http://essdepots.com:82/KTproj/update.php"));
    tmp.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return tmp;
}();

const QMetaEnum piLocalDBWorkerVarSet::DataMetaEnum = QMetaEnum::fromType<piLocalDBWorkerVarSet::Data>();
const QMetaEnum piLocalDBWorkerVarSet::ErrorMetaEnum = QMetaEnum::fromType<piLocalDBWorkerVarSet::Error>();
const QMetaEnum piLocalDBWorkerVarSet::WarningMetaEnum = QMetaEnum::fromType<piLocalDBWorkerVarSet::Warning>();
const QMetaEnum piLocalDBWorkerVarSet::NotificationMetaEnum = QMetaEnum::fromType<piLocalDBWorkerVarSet::Notification>();
//const QMetaEnum piLocalDBWorkerVarSet::QSqlErrorMetaEnum = QMetaEnum::fromType<QSqlError::ErrorType>();

const GlobalSignal piLocalDBWorkerVarSet::constGlobalSignalCheckControlTable = [](){
    GlobalSignal check4ControlTask;
    check4ControlTask.Type = QVariant::fromValue(piLocalDBWorkerVarSet::checkControlTable);
    check4ControlTask.Priority = 100;
    return check4ControlTask;
}();

const QByteArray piLocalDBWorkerVarSet::constPINGJsonPackage = QByteArray("PING");
