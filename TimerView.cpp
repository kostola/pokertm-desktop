#include "TimerView.h"

#include <QtGui>

#define FONT_NAME "Helvsetica"
#define MARGIN 5.0
#define SCREEN_WIDTH  1276.0
#define SCREEN_HEIGHT 796.0
#define SIDEBANDS_WIDTH 300.0
#define TITLE_FONT QFont(FONT_NAME, 28)

qreal calcHCenter(QGraphicsItem *item, qreal lbound, qreal rbound)
{
    return lbound + (rbound - lbound - item->boundingRect().width()) / 2.0;
}

qreal calcVCenter(QGraphicsItem *item, qreal ubound, qreal lbound)
{
    return ubound + (lbound - ubound - item->boundingRect().height()) / 2.0;
}

void setFontSizeAndCenter(QGraphicsTextItem *item, const QRectF &rect)
{
    QFont font = item->font();
    for(int i = 400; i >= 15; i -= 5) {
        font.setPixelSize(i);
        item->setFont(font);
        if(item->boundingRect().width() <= rect.width() && item->boundingRect().height() <= rect.height())
            break;
    }

    item->setPos(calcHCenter(item, rect.left(), rect.right()), calcVCenter(item, rect.top(), rect.bottom()));
}

TimerView::TimerView()
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::black));

    QPen pen_white(QBrush(Qt::white), 3.0);
    QGraphicsLineItem *l_v1 = scene->addLine(SIDEBANDS_WIDTH, 0.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT, pen_white);
    QGraphicsLineItem *l_v2 = scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, 0.0, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT, pen_white);
    QGraphicsLineItem *l_h1 = scene->addLine(0.0, SCREEN_HEIGHT / 3.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0, pen_white);
    QGraphicsLineItem *l_h2 = scene->addLine(0.0, SCREEN_HEIGHT / 3.0 * 2.0, SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);
    QGraphicsLineItem *l_h3 = scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0, pen_white);
    QGraphicsLineItem *l_h4 = scene->addLine(SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, SCREEN_WIDTH, SCREEN_HEIGHT / 3.0 * 2.0, pen_white);

    QGraphicsTextItem *txt_1 = scene->addText(tr("Giocatori"), TITLE_FONT);
    txt_1->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_2 = scene->addText(tr("Avg. Stack"), TITLE_FONT);
    txt_2->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_3 = scene->addText(tr("Total Chips"), TITLE_FONT);
    txt_3->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_4 = scene->addText(tr("Inizio"), TITLE_FONT);
    txt_4->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_5 = scene->addText(tr("Tempo di gioco"), TITLE_FONT);
    txt_5->setDefaultTextColor(Qt::white);
    QGraphicsTextItem *txt_6 = scene->addText(tr("Ora"), TITLE_FONT);
    txt_6->setDefaultTextColor(Qt::white);

    txt_1->setPos(calcHCenter(txt_1, 0.0, SIDEBANDS_WIDTH), 0.0);
    txt_2->setPos(calcHCenter(txt_2, 0.0, SIDEBANDS_WIDTH), SCREEN_HEIGHT / 3.0);
    txt_3->setPos(calcHCenter(txt_3, 0.0, SIDEBANDS_WIDTH), SCREEN_HEIGHT / 3.0 * 2.0);
    txt_4->setPos(calcHCenter(txt_4, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), 0.0);
    txt_5->setPos(calcHCenter(txt_5, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0);
    txt_6->setPos(calcHCenter(txt_6, SCREEN_WIDTH - SIDEBANDS_WIDTH, SCREEN_WIDTH), SCREEN_HEIGHT / 3.0 * 2.0);

    qreal sidebands_box_w = SIDEBANDS_WIDTH - 2.0 * MARGIN;
    qreal sidebands_box_h = SCREEN_HEIGHT / 3.0 - txt_1->boundingRect().height() - 2.0 * MARGIN;

    QRectF box_1(MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    QRectF box_2(MARGIN, SCREEN_HEIGHT / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    QRectF box_3(MARGIN, SCREEN_HEIGHT / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    QRectF box_4(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    QRectF box_5(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, SCREEN_HEIGHT / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    QRectF box_6(SCREEN_WIDTH - SIDEBANDS_WIDTH + MARGIN, SCREEN_HEIGHT / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);

    QGraphicsTextItem *txt_players = scene->addText("88/88", QFont(FONT_NAME));
    txt_players->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_players, box_1);

    QGraphicsTextItem *txt_avgstack = scene->addText("5.000", QFont(FONT_NAME));
    txt_avgstack->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_avgstack, box_2);

    QGraphicsTextItem *txt_totchips = scene->addText("40.000", QFont(FONT_NAME));
    txt_totchips->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_totchips, box_3);

    QGraphicsTextItem *txt_starttime = scene->addText("01:00:21", QFont(FONT_NAME));
    txt_starttime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_starttime, box_4);

    QGraphicsTextItem *txt_playtime = scene->addText("00:10:50", QFont(FONT_NAME));
    txt_playtime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_playtime, box_5);

    txt_time = scene->addText(QTime::currentTime().toString("HH:mm:ss"), QFont(FONT_NAME));
    txt_time->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_time, box_6);

    int button_size = (int) ((SCREEN_WIDTH - 2.0 * SIDEBANDS_WIDTH - 3.0 * MARGIN) / 7.0 - MARGIN);
    int pbcounter = 0;

    QGraphicsLinearLayout *lay_buttons = new QGraphicsLinearLayout(Qt::Horizontal);

    QPushButton *pb_play = new QPushButton("Play");
    QPalette pal = pb_play->palette();
    pal.setColor(QPalette::Window, Qt::darkGray);
    pal.setColor(QPalette::Button, QColor(0x20,0x20,0x20));
    pal.setColor(QPalette::ButtonText, Qt::white);
    /*pal.setColor(QPalette::Light, Qt::black);
    pal.setColor(QPalette::Midlight, Qt::black);
    pal.setColor(QPalette::Dark, Qt::black);
    pal.setColor(QPalette::Mid, Qt::black);
    pal.setColor(QPalette::Shadow, Qt::black);
    */
    pb_play->setPalette(pal);
    pb_play->setFlat(true);
    pb_play->setStyleSheet("border: 0px;");
    pb_play->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_play_item = scene->addWidget(pb_play);
    lay_buttons->addItem(pb_play_item);
    //pb_play_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_prev = new QPushButton("Prev");
    pb_prev->setPalette(pal);
    pb_prev->setFlat(true);
    pb_prev->setStyleSheet("border: 0px;");
    pb_prev->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_prev_item = scene->addWidget(pb_prev);
    lay_buttons->addItem(pb_prev_item);
    //pb_prev_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_next = new QPushButton("Next");
    pb_next->setPalette(pal);
    pb_next->setFlat(true);
    pb_next->setStyleSheet("border: 0px;");
    pb_next->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_next_item = scene->addWidget(pb_next);
    lay_buttons->addItem(pb_next_item);
    //pb_next_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_playerout = new QPushButton("Pl. Out");
    pb_playerout->setPalette(pal);
    pb_playerout->setFlat(true);
    pb_playerout->setStyleSheet("border: 0px;");
    pb_playerout->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_playerout_item = scene->addWidget(pb_playerout);
    lay_buttons->addItem(pb_playerout_item);
    //pb_playerout_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_rebuy = new QPushButton("Rebuy");
    pb_rebuy->setPalette(pal);
    pb_rebuy->setFlat(true);
    pb_rebuy->setStyleSheet("border: 0px;");
    pb_rebuy->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_rebuy_item = scene->addWidget(pb_rebuy);
    lay_buttons->addItem(pb_rebuy_item);
    //pb_rebuy_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_minimize = new QPushButton("Minimize");
    pb_minimize->setPalette(pal);
    pb_minimize->setFlat(true);
    pb_minimize->setStyleSheet("border: 0px;");
    pb_minimize->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_minimize_item = scene->addWidget(pb_minimize);
    lay_buttons->addItem(pb_minimize_item);
    //pb_minimize_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QPushButton *pb_close = new QPushButton("Close");
    pb_close->setPalette(pal);
    pb_close->setFlat(true);
    pb_close->setStyleSheet("border: 0px;");
    pb_close->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_close_item = scene->addWidget(pb_close);
    lay_buttons->addItem(pb_close_item);
    //pb_close_item->setPos(SIDEBANDS_WIDTH + MARGIN * (pbcounter + 1) + button_size * pbcounter, 200.0);
    //pbcounter++;

    QGraphicsWidget *buttons_item = new QGraphicsWidget();
    buttons_item->setLayout(lay_buttons);

    qDebug() << buttons_item->boundingRect();

    scene->addItem(buttons_item);
    buttons_item->setPos(SIDEBANDS_WIDTH, SCREEN_HEIGHT - button_size - 3.0 * MARGIN);

    QObject::connect(pb_play, SIGNAL(clicked()), this, SLOT(slotto()));
    QObject::connect(pb_minimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    QObject::connect(pb_close, SIGNAL(clicked()), this, SLOT(close()));

    QRectF box_timer(SIDEBANDS_WIDTH + MARGIN, MARGIN, SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), SCREEN_HEIGHT / 3.0 * 1.2);
    QRectF box_blinds(SIDEBANDS_WIDTH + MARGIN, box_timer.bottom(), SCREEN_WIDTH - 2.0 * (SIDEBANDS_WIDTH + MARGIN), SCREEN_HEIGHT / 3.0);

    QGraphicsTextItem *txt_timer = scene->addText("88:88", QFont(FONT_NAME));
    txt_timer->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_timer, box_timer);

    QGraphicsTextItem *txt_blinds = scene->addText("2000 / 4000\nAnte 200", QFont(FONT_NAME));
    txt_blinds->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(txt_blinds, box_blinds);

    this->setScene(scene);
    this->setSceneRect(0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setWindowTitle("PokerTimer - Torneo");

    QTimer *time_timer = new QTimer(this);
    QObject::connect(time_timer, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
    time_timer->start(1000);
}

void TimerView::slotto()
{
    qDebug() << "PLAY CLICKED";
}

void TimerView::updateCurrentTime()
{
    txt_time->setPlainText(QTime::currentTime().toString("HH:mm:ss"));
}
