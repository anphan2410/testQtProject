#include <QCoreApplication>
#include "databaseworker.h"
#include "QThread"
#include "QSqlDatabase"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    DatabaseWorker *worker1 = new DatabaseWorker("worker1","myconnection");
    DatabaseWorker *worker2 = new DatabaseWorker("worker2","myconnection");
    DatabaseWorker *worker3 = new DatabaseWorker("worker3","myconnection");
    DatabaseWorker *worker4 = new DatabaseWorker("worker4","myconnection");

    QThread *thread1 = new QThread();
    QThread *thread2 = new QThread();
    QThread *thread3 = new QThread();
    QThread *thread4 = new QThread();


    QObject::connect(thread1, SIGNAL(started()), worker1,SLOT(start()));
    QObject::connect(thread2, SIGNAL(started()), worker2,SLOT(start()));
    QObject::connect(thread3, SIGNAL(started()), worker3,SLOT(start()));
    QObject::connect(thread4, SIGNAL(started()), worker4,SLOT(start()));

    worker1->moveToThread(thread1);
    worker2->moveToThread(thread2);
    worker3->moveToThread(thread3);
    worker4->moveToThread(thread4);

    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();
    return a.exec();
}
