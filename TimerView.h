#ifndef TIMERVIEW_H
#define TIMERVIEW_H

#include <QGraphicsView>
#include <QObject>

#include "Tournament.h"

class TimerView : public QGraphicsView
{
    Q_OBJECT

    public:
        TimerView(Tournament *t);

    private:
        class QGraphicsTextItem *txt_time;

    private slots:
        void slotto();
        void updateCurrentTime();
};

#endif // TIMERVIEW_H
