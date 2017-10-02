#ifndef EMITREADV_H
#define EMITREADV_H

#include <QState>
#include <QTimer>
#include "uhvpvicollectordb.h"

class emitReadV : public QState
{
public:
    emitReadV(UHVPVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHVPVICollectorDB * dbPtr = Q_NULLPTR;
};

#endif // EMITREADV_H
