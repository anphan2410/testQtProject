#include "commonthings.h"

const QString piLocalDBWorkerObjName = QStringLiteral("pilambdaReturnWorker");
const QString UHV2WorkerObjName = QStringLiteral("UHV2Worker");
const QString UHV4WorkerObjName = QStringLiteral("UHV4Worker");
const QString UHV2PVICollectorObjName = QStringLiteral("UHV2PVICollector");
const QString UHV4PVICollectorObjName = QStringLiteral("UHV4PVICollector");
const QString SmallCoordinatorObjName = QStringLiteral("SmallCoordinator");
const QSqlDatabase &localQSqlDatabase = [](){
    QSqlDatabase lambdaReturn = QSqlDatabase::addDatabase("QMYSQL","originalLocalQSqlDatabaseConnection");
    lambdaReturn.setHostName("localhost");
    lambdaReturn.setDatabaseName("raspberry");
    lambdaReturn.setUserName("root");
    lambdaReturn.setPassword("Ascenx123");
    lambdaReturn.setPort(3306);
    if (lambdaReturn.open())
    {
        anAck("Local Database Connected !");
    }
    else
    {
        anError("Failed To Connect Local Database !");
        exit(EXIT_FAILURE);
    }
    return lambdaReturn;
}();
