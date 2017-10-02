#ifndef EMITREADP_H
#define EMITREADP_H

#include <QState>
#include <QTimer>
#include "uhvpvicollectordb.h"

class emitReadP : public QState
{
public:
    emitReadP(UHVPVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHVPVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADP_H
