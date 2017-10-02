#ifndef UHVPVICOLLECTORDB_H
#define UHVPVICOLLECTORDB_H

#define UHVPVICollectorDBDbgEn 1
#define UHVPVICollectorStateDbgEn 1

#include <QObject>
#include <QStateMachine>
#include "anlogger.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QByteArray>
#include "commonthings.h"
#include "binaryprotocol.h"
#include "windowprotocol.h"
#include "SerialPortWorker/SerialPortWorker/serialportworkerproperty.h"
#include "piLocalDBWorker/piLocalDBWorker/pilocaldbworkervarset.h"

class UHVPVICollectorDB : public QObject
{
    Q_OBJECT
public:
    explicit UHVPVICollectorDB(bool isUHV2, QObject *parent = nullptr);

    enum Data {
        NoData = 0,
        pauseMachine,
        resumeMachine
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0
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

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;
    static const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

    quint32 currentGlobalID;
    quint32 breakIntervalMSecs = 1000;
    quint32 waitIntervalMSecs = 5000;
    QSqlDatabase localDb;
    QSqlQuery currentQuery;
    QByteArray currentReplyFromPump;
    QString currentPressure;
    QString currentVoltage;
    QString currentCurrent;
    QString previousReadState;
    quint8 currentPNo;
    quint8 currentCH;
    bool isAnUHV2;
    bool isReady = false;
    QByteArray QBAReadP;
    QByteArray QBAReadV;
    QByteArray QBAReadI;
    GlobalSignal GS2UHV;
signals:
    void Out(const GlobalSignal &);
    void errorOccurred();
    void DatabaseClosed();
    void pause();
    void directTransitionRequest(const QString &);
    void SignalToUHVEmitted();
    void DataFromUHVObtained();
    void MessageToDatabase(quint32 GlobalID, const QString &Pressure, const QString &Voltage, const QString &Current);
public slots:
    void initialize();
    bool connectDatabase();
    bool gotoNextRecord();
    void emitReadP();
    void emitReadV();
    void emitReadI();
    void emitMsgToDatabaseUpdatePVI();
    void saveP();
    void saveV();
    void saveI();
    void setPreviousReadState(const QString &StateObjName);
    void resume();
    void processDataFromPump(const QByteArray &data);
private:
    BinaryProtocol currentBP;
    WindowProtocol currentWP;
    QString getDataString();
    void closeDatabaseConnection();
};

#endif // UHVPVICOLLECTORDB_H
