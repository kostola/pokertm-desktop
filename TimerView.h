#ifndef TIMERVIEW_H
#define TIMERVIEW_H

#include <QGraphicsView>
#include <QObject>

class TimerView : public QGraphicsView
{
    Q_OBJECT

    public:
        TimerView();

    private slots:
        void slotto();
};

#endif // TIMERVIEW_H
