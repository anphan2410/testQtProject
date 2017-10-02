#ifndef WAIT4I_H
#define WAIT4I_H

#include <QState>
#include <QTimer>
#include "uhvpvicollectordb.h"

class wait4I : public QState
{
public:
    wait4I(UHVPVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
    UHVPVICollectorDB * dbPtr = Q_NULLPTR;
};
#endif // WAIT4I_H
