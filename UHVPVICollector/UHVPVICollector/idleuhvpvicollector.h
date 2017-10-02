#ifndef IDLEUHVPVICOLLECTOR_H
#define IDLEUHVPVICOLLECTOR_H

#include <QState>
#include "uhvpvicollectordb.h"

class idleUHVPVICollector : public QState
{
public:
    idleUHVPVICollector(UHVPVICollectorDB *database);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    UHVPVICollectorDB *dbPtr = Q_NULLPTR;
};

#endif // IDLEUHVPVICOLLECTOR_H
