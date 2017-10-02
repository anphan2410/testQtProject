#ifndef WAIT4P_H
#define WAIT4P_H

#include <QState>
#include <QTimer>
#include "uhvpvicollectordb.h"

class wait4P : public QState
{
public:
    wait4P(UHVPVICollectorDB * database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    QTimer timer;
};

#endif // WAIT4P_H
