#ifndef CONNECTDATABASE_H
#define CONNECTDATABASE_H

#include <QState>
#include "pilocaldbworkervarset.h"

class connectDatabase : public QState
{
public:
    connectDatabase(piLocalDBWorkerVarSet * VarSet, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerVarSet * VarSetPtr;
};

#endif // CONNECTDATABASE_H
