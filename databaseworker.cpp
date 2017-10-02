#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(const QString &threadname, const QString &connectionName, QObject *parent) : QObject(parent), m_threadName(threadname), m_connectionName(connectionName)
{

}


void DatabaseWorker::start()
{


    anTrk("Thread: " + m_threadName + "--------------");
    LocalDB = QSqlDatabase::cloneDatabase(uniqueLocalDB, m_threadName);
    if(LocalDB.open())
    {
        anAck("Connected to Database");
        aTimer = new QTimer(this);
        aTimer->setInterval(100);
        QObject::connect(aTimer, SIGNAL(timeout()), this, SLOT(execQuery()));
        aTimer->start();
        anAck("Timer Start");
    }
    else
    {
        anAck("Could not Connect to Database");
    }

}

void DatabaseWorker::execQuery()
{
    anError("Thread: " + m_threadName + "--------------");

    QSqlQuery tmpQuery(LocalDB);

    if(tmpQuery.exec("SELECT * FROM stations WHERE GlobalID=4"))
    {
        if(tmpQuery.first())
        {
            anAck("OK");
        }
        else
        {
            anError("Query return empty");
        }
    }
    else
    {
        anError("Querry failed");
        anError(QSqlError(tmpQuery.lastError()).text());
    }


}
