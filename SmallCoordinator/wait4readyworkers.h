#ifndef WAIT4READYWORKERS_H
#define WAIT4READYWORKERS_H

#include <QState>
#include "smallcoordinatordb.h"

class wait4ReadyWorkers : public QState
{
public:
    wait4ReadyWorkers(QState * parent);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
};

#endif // WAIT4READYWORKERS_H
