#include "TimerView.h"

#include <QtGui>

#define SCREEN_WIDTH  1276.0
#define SCREEN_HEIGHT 796.0

#define TITLEFONT QFont("Helvetica", 30)

qreal calcHCenter(QGraphicsItem *item, qreal lbound, qreal rbound)
{
    return lbound + (rbound - lbound - item->boundingRect().width()) / 2.0;
}

qreal calcVCenter(QGraphicsItem *item, qreal ubound, qreal lbound)
{
    return ubound + (lbound - ubound - item->boundingRect().height()) / 2.0;
}

TimerView::TimerView()
{
    qreal sidebands_width = 300.0;

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::black));

    QGraphicsTextItem *txt_1 = scene->addText(tr("Giocatori"), TITLEFONT);
    txt_1->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_2 = scene->addText(tr("Avg. Stack"), TITLEFONT);
    txt_2->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_3 = scene->addText(tr("Total Chips"), TITLEFONT);
    txt_3->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_4 = scene->addText(tr("Inizio"), TITLEFONT);
    txt_4->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_5 = scene->addText(tr("Tempo di gioco"), TITLEFONT);
    txt_5->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_6 = scene->addText(tr("Ora"), TITLEFONT);
    txt_6->setDefaultTextColor(Qt::white);

    txt_1->setPos(calcHCenter(txt_1, 0.0, sidebands_width), 15.0);
    txt_2->setPos(calcHCenter(txt_2, 0.0, sidebands_width), SCREEN_HEIGHT / 3.0 + 15.0);
    txt_3->setPos(calcHCenter(txt_3, 0.0, sidebands_width), SCREEN_HEIGHT / 3.0 * 2.0 + 15.0);
    txt_4->setPos(calcHCenter(txt_4, SCREEN_WIDTH - sidebands_width, SCREEN_WIDTH), 15.0);
    txt_5->setPos(calcHCenter(txt_5, SCREEN_WIDTH - sidebands_width, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0 + 15.0);
    txt_6->setPos(calcHCenter(txt_6, SCREEN_WIDTH - sidebands_width, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0 * 2.0 + 15.0);

    QPen pen_white(QBrush(Qt::white), 3.0);
    QGraphicsLineItem *l_v1 = scene->addLine(sidebands_width, 0.0, sidebands_width, SCREEN_HEIGHT, pen_white);
    QGraphicsLineItem *l_v2 = scene->addLine(SCREEN_WIDTH - sidebands_width, 0.0, SCREEN_WIDTH - sidebands_width, SCREEN_HEIGHT, pen_white);
    QGraphicsLineItem *l_h1 = scene->addLine(0.0, SCREEN_HEIGHT / 3.0, sidebands_width, SCREEN_HEIGHT / 3.0, pen_white);
    QGraphicsLineItem *l_h2 = scene->addLine(0.0, SCREEN_HEIGHT / 3.0 * 2.0, sidebands_width, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);
    QGraphicsLineItem *l_h3 = scene->addLine(SCREEN_WIDTH - sidebands_width, SCREEN_HEIGHT / 3.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0, pen_white);
    QGraphicsLineItem *l_h4 = scene->addLine(SCREEN_WIDTH - sidebands_width, SCREEN_HEIGHT / 3.0 * 2.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);

    this->setScene(scene);
    this->setSceneRect(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
