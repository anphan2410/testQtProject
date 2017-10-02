#include <QApplication>
#include "mainwindow.h"
#include <QThread>
#include "piLocalDBWorker/piLocalDBWorker/pilocaldbworker.h"
#include "SerialPortWorker/SerialPortWorker/serialportworker.h"
#include "UHVPVICollector/UHVPVICollector/uhvpvicollector.h"
#include "SmallCoordinator/smallcoordinator.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<GlobalSignal>("GlobalSignal");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SerialPortWorker * uhv2worker = new SerialPortWorker();
    uhv2worker->setObjectName(UHV2WorkerObjName);
    SerialPortWorker * uhv4worker = new SerialPortWorker();
    uhv4worker->setObjectName(UHV4WorkerObjName);
    UHVPVICollector * uhv2pvicollector = new UHVPVICollector(true);
    uhv2pvicollector->setObjectName(UHV2PVICollectorObjName);
    UHVPVICollector * uhv4pvicollector = new UHVPVICollector(false);
    uhv4pvicollector->setObjectName(UHV4PVICollectorObjName);
    piLocalDBWorker * piLocalDatabase = new piLocalDBWorker();
    piLocalDatabase->setObjectName(piLocalDBWorkerObjName);
    SmallCoordinator * smallcoordinator = new SmallCoordinator();
    smallcoordinator->setObjectName(SmallCoordinatorObjName);

    QObject::connect(uhv2worker, &SerialPortWorker::Out, smallcoordinator, &SmallCoordinator::In);
    QObject::connect(uhv4worker, &SerialPortWorker::Out, smallcoordinator, &SmallCoordinator::In);
    QObject::connect(piLocalDatabase, &piLocalDBWorker::Out, smallcoordinator, &SmallCoordinator::In);
    QObject::connect(uhv2pvicollector, &UHVPVICollector::Out, smallcoordinator, &SmallCoordinator::In);
    QObject::connect(uhv4pvicollector, &UHVPVICollector::Out, smallcoordinator, &SmallCoordinator::In);

    QObject::connect(smallcoordinator, &SmallCoordinator::ToPiLocalDBWorker, piLocalDatabase, &piLocalDBWorker::In);
    QObject::connect(smallcoordinator, &SmallCoordinator::ToUHV2Worker, uhv2worker, &SerialPortWorker::In);
    QObject::connect(smallcoordinator, &SmallCoordinator::ToUHV4Worker, uhv4worker, &SerialPortWorker::In);
    QObject::connect(smallcoordinator, &SmallCoordinator::ToUHV2PVICollector, uhv2pvicollector, &UHVPVICollector::In);
    QObject::connect(smallcoordinator, &SmallCoordinator::ToUHV4PVICollector, uhv4pvicollector, &UHVPVICollector::In);

    QThread * uhv2workerThread = new QThread();
    QThread * uhv4workerThread = new QThread();
    QThread * uhv2pvicollectorThread = new QThread();
    QThread * uhv4pvicollectorThread = new QThread();
    QThread * piLocalDatabaseThread = new QThread();
    QThread * smallcoordinatorThread = new QThread();

    uhv2worker->moveToThread(uhv2workerThread);
    uhv4worker->moveToThread(uhv4workerThread);
    uhv2pvicollector->moveToThread(uhv2pvicollectorThread);
    uhv4pvicollector->moveToThread(uhv4pvicollectorThread);
    piLocalDatabase->moveToThread(piLocalDatabaseThread);
    smallcoordinator->moveToThread(smallcoordinatorThread);

    QObject::connect(uhv2workerThread, &QThread::started, uhv2worker, &SerialPortWorker::start);
    QObject::connect(uhv4workerThread, &QThread::started, uhv4worker, &SerialPortWorker::start);
    QObject::connect(uhv2pvicollectorThread, &QThread::started, uhv2pvicollector, &UHVPVICollector::start);
    QObject::connect(uhv4pvicollectorThread, &QThread::started, uhv4pvicollector, &UHVPVICollector::start);
    QObject::connect(piLocalDatabaseThread, &QThread::started, piLocalDatabase, &piLocalDBWorker::start);
    QObject::connect(smallcoordinatorThread, &QThread::started, smallcoordinator, &SmallCoordinator::start);

    QObject::connect(smallcoordinator, &SmallCoordinator::getReady, [&](){
        piLocalDatabaseThread->start();
        uhv2workerThread->start();
        uhv4workerThread->start();
        uhv2pvicollectorThread->start();
        uhv4pvicollectorThread->start();
    });

    smallcoordinatorThread->start();


    return a.exec();
}
