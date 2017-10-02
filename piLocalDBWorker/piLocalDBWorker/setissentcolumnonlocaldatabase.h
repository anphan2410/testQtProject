#ifndef SETISSENTCOLUMNONLOCALDATABASE_H
#define SETISSENTCOLUMNONLOCALDATABASE_H

#include <QState>
#include "pilocaldbworkervarset.h"

class setIsSentColumnOnLocalDatabase : public QState
{
public:
    setIsSentColumnOnLocalDatabase(piLocalDBWorkerVarSet * VarSet, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerVarSet * VarSetPtr;
};

#endif // SETISSENTCOLUMNONLOCALDATABASE_H
