#ifndef PILOCALDBWORKERVARSET_H
#define PILOCALDBWORKERVARSET_H

#define piLocalDBWorkerVarSetDbgEn 1

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QUrl>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork>
#include <QNetworkReply>
#include <QMetaType>
#include <QMetaEnum>
#include "binaryprotocol.h"
#include "windowprotocol.h"
#include "canprotocol.h"
#include "anlogger.h"

class piLocalDBWorkerVarSet : public QObject
{
    Q_OBJECT
public:
    explicit piLocalDBWorkerVarSet(QObject *parent = nullptr);

    enum Data
    {
        NoData = 0,
        updatePVIData,
        updateRFIDData,
        checkControlTable,
        syncOfflineData
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        QSqlError
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        readyToWork
    };
    Q_ENUM(Notification)

    typedef struct {
        quint32 GlobalID;
        QString Pressure;
        QString Voltage;
        QString Current;
    } PVIData;

    typedef struct {
        quint32 GlobalID;
        QString srcRFID;
    } RFIDData;

    void initialize();
    void dispose();
    bool connectLocalDatabase();
    void closeLocalDatabaseConnection();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void addOneGlobalSignal(const GlobalSignal &aGlobalSignal);
    GlobalSignal &takeOutFirstOfMostPrioritizedGlobalSignals();
    GlobalSignal &takeOutLastOfMostPrioritizedGlobalSignals();
    void clearEmptyList();
    void clearAllReservedStrings();

    bool isOnline = true;
    QSqlDatabase localDb;
    Error ErrorType = NoError;
    QString ErrorInfo;

    QMap<quint8,QList<GlobalSignal>*> prioritizedBuffer;

    QNetworkReply * networkReply = Q_NULLPTR;

    bool isPendingJsonDataNotPING();

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
//    static const QMetaEnum QSqlErrorMetaEnum;
    static const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

    void sendJsonDataPackage(const QByteArray &data);
    bool sendPendingJsonDataPackage();
    const QJsonObject createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time);
    const QJsonArray getPendingJsonDataPackage();
signals:
    void Out(const GlobalSignal &);
    void ErrorOccurred();
    void DatabaseConnected();
    void DatabaseClosed();
    void firstGlobalSignalAdded();
    void oneGlobalSignalAdded();
    void directTransitionRequest(const QString &);
    void jsonPackageTransmitted();    

    void controlUHV2(BinaryProtocol);
    void controlUHV4(WindowProtocol);
    void controlValve(CanProtocol);
public slots:
    void requestControlTableCheck();
    void updateLocalDatabase();
    void setIsSentColumnOnLocalDatabase();
    void emitErrorGlobalSignal();
private:    
    static const QByteArray constPINGJsonPackage;
    static const QNetworkRequest onlineDb;
    QNetworkAccessManager NAManager;

    QSqlQuery instantQuery;

    //temporary variables
    GlobalSignal * currentGlobalSignal;
    QSqlQuery sourceQuery;
    QSqlQuery tmpQuery;
    QSqlQuery tmpQuery2;
    QString tmpStr;
    QString tmpStr1;
    QString tmpStr2;
    QString tmpStr3;
    QString tmpStr4;
    QString tmpStr5;
    QString reservedStr1;
    QString reservedStr2;
    QByteArray pendingJSonData;
    BinaryProtocol tmpBP;
    WindowProtocol tmpWP;
    CanProtocol tmpCP;

    static const GlobalSignal constGlobalSignalCheckControlTable;
};

Q_DECLARE_METATYPE(piLocalDBWorkerVarSet::PVIData)
Q_DECLARE_METATYPE(piLocalDBWorkerVarSet::RFIDData)
#endif // PILOCALDBWORKERVARSET_H
