#-------------------------------------------------
#
# Project created by QtCreator 2017-09-11T11:46:32
#
#-------------------------------------------------

QT       += core gui serialport serialbus sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += -std=c++11 console
TARGET = ExecSmallCoordinator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    binaryprotocol.cpp \
    canprotocol.cpp \
    windowprotocol.cpp \
    SmallCoordinator/smallcoordinator.cpp \
    SmallCoordinator/smallcoordinatordb.cpp \
    SmallCoordinator/coordinateglobalsignals.cpp \
    SmallCoordinator/wait4readyworkers.cpp \
    piLocalDBWorker/piLocalDBWorker/connectdatabase.cpp \
    piLocalDBWorker/piLocalDBWorker/directtransition4pilocaldbworkerstate.cpp \
    piLocalDBWorker/piLocalDBWorker/pilocaldbworker.cpp \
    piLocalDBWorker/piLocalDBWorker/pilocaldbworkervarset.cpp \
    piLocalDBWorker/piLocalDBWorker/setissentcolumnonlocaldatabase.cpp \
    piLocalDBWorker/piLocalDBWorker/updatelocaldatabase.cpp \
    piLocalDBWorker/piLocalDBWorker/updateonlinedatabase.cpp \
    piLocalDBWorker/piLocalDBWorker/wait4errorhandler4pilocaldbworker.cpp \
    SerialPortWorker/SerialPortWorker/connectserialport.cpp \
    SerialPortWorker/SerialPortWorker/directtransition4serialportworkerstate.cpp \
    SerialPortWorker/SerialPortWorker/readbytes.cpp \
    SerialPortWorker/SerialPortWorker/requestserialportinfo.cpp \
    SerialPortWorker/SerialPortWorker/serialportworker.cpp \
    SerialPortWorker/SerialPortWorker/serialportworkerproperty.cpp \
    SerialPortWorker/SerialPortWorker/validateserialportinfo.cpp \
    SerialPortWorker/SerialPortWorker/wait4errorhandler4serialportworker.cpp \
    SerialPortWorker/SerialPortWorker/writebytes.cpp \
    UHVPVICollector/UHVPVICollector/directtransitionforuhvpvicollectorstate.cpp \
    UHVPVICollector/UHVPVICollector/emitreadi.cpp \
    UHVPVICollector/UHVPVICollector/emitreadp.cpp \
    UHVPVICollector/UHVPVICollector/emitreadv.cpp \
    UHVPVICollector/UHVPVICollector/idleuhvpvicollector.cpp \
    UHVPVICollector/UHVPVICollector/uhvpvicollector.cpp \
    UHVPVICollector/UHVPVICollector/uhvpvicollectordb.cpp \
    UHVPVICollector/UHVPVICollector/wait4i.cpp \
    UHVPVICollector/UHVPVICollector/wait4p.cpp \
    UHVPVICollector/UHVPVICollector/wait4v.cpp \
    SmallCoordinator/directtransitionforsmallcoordinatorstate.cpp \
    SmallCoordinator/wait4errorhandler4smallcoordinator.cpp \
    commonthings.cpp \
    anlogger.cpp

HEADERS += \
        mainwindow.h \
    anlogger.h \
    binaryprotocol.h \
    canprotocol.h \
    commonthings.h \
    windowprotocol.h \
    SmallCoordinator/smallcoordinator.h \
    SmallCoordinator/smallcoordinatordb.h \
    SmallCoordinator/coordinateglobalsignals.h \
    SmallCoordinator/wait4readyworkers.h \
    piLocalDBWorker/piLocalDBWorker/connectdatabase.h \
    piLocalDBWorker/piLocalDBWorker/directtransition4pilocaldbworkerstate.h \
    piLocalDBWorker/piLocalDBWorker/pilocaldbworker.h \
    piLocalDBWorker/piLocalDBWorker/pilocaldbworkervarset.h \
    piLocalDBWorker/piLocalDBWorker/setissentcolumnonlocaldatabase.h \
    piLocalDBWorker/piLocalDBWorker/updatelocaldatabase.h \
    piLocalDBWorker/piLocalDBWorker/updateonlinedatabase.h \
    piLocalDBWorker/piLocalDBWorker/wait4errorhandler4pilocaldbworker.h \
    SerialPortWorker/SerialPortWorker/connectserialport.h \
    SerialPortWorker/SerialPortWorker/directtransition4serialportworkerstate.h \
    SerialPortWorker/SerialPortWorker/readbytes.h \
    SerialPortWorker/SerialPortWorker/requestserialportinfo.h \
    SerialPortWorker/SerialPortWorker/serialportworker.h \
    SerialPortWorker/SerialPortWorker/serialportworkerproperty.h \
    SerialPortWorker/SerialPortWorker/validateserialportinfo.h \
    SerialPortWorker/SerialPortWorker/wait4errorhandler4serialportworker.h \
    SerialPortWorker/SerialPortWorker/writebytes.h \
    UHVPVICollector/UHVPVICollector/directtransitionforuhvpvicollectorstate.h \
    UHVPVICollector/UHVPVICollector/emitreadi.h \
    UHVPVICollector/UHVPVICollector/emitreadp.h \
    UHVPVICollector/UHVPVICollector/emitreadv.h \
    UHVPVICollector/UHVPVICollector/idleuhvpvicollector.h \
    UHVPVICollector/UHVPVICollector/uhvpvicollector.h \
    UHVPVICollector/UHVPVICollector/uhvpvicollectordb.h \
    UHVPVICollector/UHVPVICollector/wait4i.h \
    UHVPVICollector/UHVPVICollector/wait4p.h \
    UHVPVICollector/UHVPVICollector/wait4v.h \
    SmallCoordinator/directtransitionforsmallcoordinatorstate.h \
    SmallCoordinator/wait4errorhandler4smallcoordinator.h

FORMS += \
        mainwindow.ui
