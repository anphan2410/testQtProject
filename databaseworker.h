#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include "abcheaderonce.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QSqlError>

class DatabaseWorker: public QObject
{

    Q_OBJECT

public:
    DatabaseWorker(const QString &threadname, const QString &connectionName, QObject *parent = 0);

public slots:
    void start();
    void execQuery();

private:
    QSqlDatabase LocalDB;
    QString m_threadName;
    QString m_connectionName;
    QTimer *aTimer;
};

#endif // DATABASEWORKER_H
