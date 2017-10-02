#ifndef UPDATEONLINEDATABASE_H
#define UPDATEONLINEDATABASE_H

#include <QState>
#include <QTimer>
#include "pilocaldbworkervarset.h"

class updateOnlineDatabase : public QState
{
public:
    updateOnlineDatabase(piLocalDBWorkerVarSet * VarSet, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer sendJsonTimer;
    piLocalDBWorkerVarSet * VarSetPtr;
};

#endif // UPDATEONLINEDATABASE_H
