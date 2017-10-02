#ifndef UPDATELOCALDATABASE_H
#define UPDATELOCALDATABASE_H

#include <QState>
#include <QTimer>
#include "pilocaldbworkervarset.h"

class updateLocalDatabase : public QState
{
public:
    updateLocalDatabase(piLocalDBWorkerVarSet * VarSet, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer checkControlTask;
    piLocalDBWorkerVarSet * VarSetPtr;
};

#endif // UPDATELOCALDATABASE_H
