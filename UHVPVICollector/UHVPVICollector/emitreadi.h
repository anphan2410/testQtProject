#ifndef EMITREADI_H
#define EMITREADI_H

#include <QState>
#include <QTimer>
#include "uhvpvicollectordb.h"

class emitReadI : public QState
{
public:
    emitReadI(UHVPVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHVPVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADI_H
