#ifndef COORDINATEGLOBALSIGNALS_H
#define COORDINATEGLOBALSIGNALS_H

#include <QState>
#include <QTimer>
#include "smallcoordinatordb.h"

class coordinateGlobalSignals : public QState
{
public:
    coordinateGlobalSignals(SmallCoordinatorDB * database, QState *parent = Q_NULLPTR);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    SmallCoordinatorDB * dbPtr;
    QTimer timer;
};

#endif // COORDINATEGLOBALSIGNALS_H
