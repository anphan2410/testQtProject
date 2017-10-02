#ifndef SMALLCOORDINATORDB_H
#define SMALLCOORDINATORDB_H

#define SmallCoordinatorDBDbgEn 1

#include <QObject>
#include "anlogger.h"
#include "commonthings.h"
#include "piLocalDBWorker/piLocalDBWorker/pilocaldbworkervarset.h"
#include "SerialPortWorker/SerialPortWorker/serialportworkerproperty.h"
#include "UHVPVICollector/UHVPVICollector/uhvpvicollectordb.h"

class SmallCoordinatorDB : public QObject
{
    Q_OBJECT
public:
    explicit SmallCoordinatorDB(QObject *parent = nullptr);

    enum Data
    {
        NoData = 0,
        pauseAllCollectors,
        resumeAllCollectors,
        resumeUHVPVICollectors
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
        NoNotification = 0
    };
    Q_ENUM(Notification)

    void initialize();
    void dispose();
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    void clearError();
    void addOneGlobalSignal(const GlobalSignal &aGlobalSignal);
    GlobalSignal &takeOutFirstOfMostPrioritizedGlobalSignals();
    GlobalSignal &takeOutLastOfMostPrioritizedGlobalSignals();
    void clearEmptyList();
    void clearCache();
    void clearPrioritizedBuffer();
    void checkIfAllWorkersReady();

    QMap<quint8,QList<GlobalSignal>> prioritizedBuffer;
    Error ErrorType = NoError;
    QString ErrorInfo;

    bool isPiLocalDBWorkerReady = false;
    bool isUHV2WorkerReady = false;
    bool isUHV4WorkerReady  =false;
    bool isUHV2PVICollectorReady = false;
    bool isUHV4PVICollectorReady = false;
    bool isAllWorkersReady = false;

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;

signals:
    void Out(const GlobalSignal &);
    void ErrorOccurred();
    void allWorkersReady();
    void firstGlobalSignalAdded();
    void oneGlobalSignalAdded();
    void requestDirectTransition(const QString &);
    void ToUHV2Worker(const GlobalSignal &);
    void ToUHV4Worker(const GlobalSignal &);
    void ToUHV2PVICollector(const GlobalSignal &);
    void ToUHV4PVICollector(const GlobalSignal &);
    void ToPiLocalDBWorker(const GlobalSignal &);
public slots:
    void In(const GlobalSignal &aGlobalSignal);
    void executeGlobalSignals();
    void emitErrorGlobalSignal();
private:
    //Cache
    GlobalSignal currentGlobalSignal;
    QString currentDestination;
};
#endif // SMALLCOORDINATORDB_H
