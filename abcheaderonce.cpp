#include "abcheaderonce.h"

const QSqlDatabase &uniqueLocalDB = [](){
    QSqlDatabase LocalDB = QSqlDatabase::addDatabase("QMYSQL");
    LocalDB.setHostName("localhost");
    LocalDB.setDatabaseName("raspberry");
    LocalDB.setUserName("root");
    LocalDB.setPassword("Ascenx123");

    if(LocalDB.open())
    {
        anAck("OK LocalDATABASE Connected !");
    }
    else
    {
        anAck("Failed To Connect Database !");
    }
    return LocalDB;
}();
