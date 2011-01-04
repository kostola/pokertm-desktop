/***************************************************************************
 *                                                                         *
 *   This file is part of the PokerTournamentManager project,              *
 *       http://www.gitorious.org/poker-tournament-manager                 *
 *                                                                         *
 *   Authors: Alessandro Costa <kost.on.linux@gmail.com>, (C) 2009         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation. For more details see the   *
 *   COPYING.txt file that comes with this distribution.                   *
 *                                                                         *
 ***************************************************************************/

#include "TimerView.h"

#include <QtGui>
#include <QApplication>

#include "Global.h"

#define MARGIN 5.0
#define FONT_NAME "Helvetica"

// ===== FUNCTIONS ================================================================================

QString addNumberPoints(const QString& n)
{
    QString num;
    int counter = 0;
    for(int i = n.size() - 1; i >= 0; i--) {
        if(counter > 0 && counter % 3 == 0)
            num.prepend(".");
        num.prepend(n.mid(i,1));
        counter++;
    }

    return num;
}

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

// ===== CLASS "TimerView" ========================================================================

TimerView::TimerView(Tournament *t)
    : m_tournament(t)
    , m_paused(true)
{
    // calculating dimensions
    m_screen_width = ((double)QApplication::desktop()->geometry().width()) - 4.0;
    m_screen_height = ((double)QApplication::desktop()->geometry().height()) - 4.0;
    m_sidebands_width = m_screen_width / 4.0 * 0.9;

    // creating scene
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::black));

    // creating bounding lines
    QPen pen_white(QBrush(Qt::white), 3.0);
    scene->addLine(m_sidebands_width, 0.0, m_sidebands_width, m_screen_height, pen_white);
    scene->addLine(m_screen_width - m_sidebands_width, 0.0, m_screen_width - m_sidebands_width, m_screen_height, pen_white);
    scene->addLine(0.0, m_screen_height / 3.0, m_sidebands_width, m_screen_height / 3.0, pen_white);
    scene->addLine(0.0, m_screen_height / 3.0 * 2.0, m_sidebands_width, m_screen_height / 3.0 * 2.0, pen_white);
    scene->addLine(m_screen_width - m_sidebands_width, m_screen_height / 3.0, m_screen_width, m_screen_height / 3.0, pen_white);
    scene->addLine(m_screen_width - m_sidebands_width, m_screen_height / 3.0 * 2.0, m_screen_width, m_screen_height / 3.0 * 2.0, pen_white);

    // calculating fixed texts point size
    int fixed_txt_size = qCeil(m_screen_width / 50.0) + 2;

    // creating fixed texts
    QGraphicsTextItem *txt_1 = scene->addText(tr("Giocatori"),      QFont(FONT_NAME, fixed_txt_size));
    QGraphicsTextItem *txt_2 = scene->addText(tr("Avg. Stack"),     QFont(FONT_NAME, fixed_txt_size));
    QGraphicsTextItem *txt_3 = scene->addText(tr("Total Chips"),    QFont(FONT_NAME, fixed_txt_size));
    QGraphicsTextItem *txt_4 = scene->addText(tr("Inizio"),         QFont(FONT_NAME, fixed_txt_size));
    QGraphicsTextItem *txt_5 = scene->addText(tr("Tempo di gioco"), QFont(FONT_NAME, fixed_txt_size));
    QGraphicsTextItem *txt_6 = scene->addText(tr("Ora"),            QFont(FONT_NAME, fixed_txt_size));

    // setting fixed texts color
    txt_1->setDefaultTextColor(Qt::white);
    txt_2->setDefaultTextColor(Qt::white);
    txt_3->setDefaultTextColor(Qt::white);
    txt_4->setDefaultTextColor(Qt::white);
    txt_5->setDefaultTextColor(Qt::white);
    txt_6->setDefaultTextColor(Qt::white);

    // positioning fixed texts
    txt_1->setPos(calcHCenter(txt_1, 0.0, m_sidebands_width), 0.0);
    txt_2->setPos(calcHCenter(txt_2, 0.0, m_sidebands_width), m_screen_height / 3.0);
    txt_3->setPos(calcHCenter(txt_3, 0.0, m_sidebands_width), m_screen_height / 3.0 * 2.0);
    txt_4->setPos(calcHCenter(txt_4, m_screen_width - m_sidebands_width, m_screen_width), 0.0);
    txt_5->setPos(calcHCenter(txt_5, m_screen_width - m_sidebands_width, m_screen_width), m_screen_height / 3.0);
    txt_6->setPos(calcHCenter(txt_6, m_screen_width - m_sidebands_width, m_screen_width), m_screen_height / 3.0 * 2.0);

    // calculating side boxes size
    qreal sidebands_box_w = m_sidebands_width - 2.0 * MARGIN;
    qreal sidebands_box_h = m_screen_height / 3.0 - txt_1->boundingRect().height() - 2.0 * MARGIN;

    // creating side boxes
    box_1 = QRectF(MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_2 = QRectF(MARGIN, m_screen_height / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_3 = QRectF(MARGIN, m_screen_height / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_4 = QRectF(m_screen_width - m_sidebands_width + MARGIN, txt_1->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_5 = QRectF(m_screen_width - m_sidebands_width + MARGIN, m_screen_height / 3.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);
    box_6 = QRectF(m_screen_width - m_sidebands_width + MARGIN, m_screen_height / 3.0 * 2.0 + txt_2->boundingRect().height() + MARGIN, sidebands_box_w, sidebands_box_h);

    // creating variable texts for side boxes
    m_txt_players = scene->addText("", QFont(FONT_NAME));
    m_txt_players->setDefaultTextColor(Qt::white);

    m_txt_avgstack = scene->addText("", QFont(FONT_NAME));
    m_txt_avgstack->setDefaultTextColor(Qt::white);

    m_txt_totchips = scene->addText("", QFont(FONT_NAME));
    m_txt_totchips->setDefaultTextColor(Qt::white);

    updateGraphics(AverageStackGraphics | PlayersGraphics | TotalChipsGraphics);

    m_txt_starttime = scene->addText("00:00:00", QFont(FONT_NAME));
    m_txt_starttime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_starttime, box_4);

    m_txt_playtime = scene->addText("00:00:00", QFont(FONT_NAME));
    m_txt_playtime->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_playtime, box_5);

    m_txt_time = scene->addText(QTime::currentTime().toString("HH:mm:ss"), QFont(FONT_NAME));
    m_txt_time->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_time, box_6);

    // calculating lower buttons size
    int button_size = (int) ((m_screen_width - 2.0 * m_sidebands_width - 3.0 * MARGIN) / 7.0 - MARGIN);

    // creating lower buttons layout
    QGraphicsLinearLayout *lay_buttons = new QGraphicsLinearLayout(Qt::Horizontal);
    lay_buttons->setSpacing(MARGIN);

    // creating lower buttons
    m_pb_play = new QPushButton();
    m_pb_play->setIcon(QIcon(":/data/icon_play.png"));
    m_pb_play->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    QPalette pal = m_pb_play->palette();
    pal.setColor(QPalette::Window, Qt::black);
    pal.setColor(QPalette::Button, Qt::black);
    pal.setColor(QPalette::ButtonText, Qt::white);
    m_pb_play->setPalette(pal);
    m_pb_play->setFlat(true);
    m_pb_play->setStyleSheet("border: 0px;");
    m_pb_play->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_play_item = scene->addWidget(m_pb_play);
    lay_buttons->addItem(pb_play_item);

    m_pb_prev = new QPushButton();
    m_pb_prev->setIcon(QIcon(":/data/icon_prev.png"));
    m_pb_prev->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_prev->setPalette(pal);
    m_pb_prev->setFlat(true);
    m_pb_prev->setStyleSheet("border: 0px;");
    m_pb_prev->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_prev_item = scene->addWidget(m_pb_prev);
    lay_buttons->addItem(pb_prev_item);

    m_pb_next = new QPushButton();
    m_pb_next->setIcon(QIcon(":/data/icon_next.png"));
    m_pb_next->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_next->setPalette(pal);
    m_pb_next->setFlat(true);
    m_pb_next->setStyleSheet("border: 0px;");
    m_pb_next->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_next_item = scene->addWidget(m_pb_next);
    lay_buttons->addItem(pb_next_item);

    m_pb_playerout = new QPushButton();
    m_pb_playerout->setIcon(QIcon(":/data/icon_plout.png"));
    m_pb_playerout->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_playerout->setPalette(pal);
    m_pb_playerout->setFlat(true);
    m_pb_playerout->setStyleSheet("border: 0px;");
    m_pb_playerout->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_playerout_item = scene->addWidget(m_pb_playerout);
    lay_buttons->addItem(pb_playerout_item);

    m_pb_rebuy = new QPushButton();
    m_pb_rebuy->setIcon(QIcon(":/data/icon_rebuy.png"));
    m_pb_rebuy->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_rebuy->setPalette(pal);
    m_pb_rebuy->setFlat(true);
    m_pb_rebuy->setStyleSheet("border: 0px;");
    m_pb_rebuy->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_rebuy_item = scene->addWidget(m_pb_rebuy);
    lay_buttons->addItem(pb_rebuy_item);

    m_pb_minimize = new QPushButton();
    m_pb_minimize->setIcon(QIcon(":/data/icon_minimize.png"));
    m_pb_minimize->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_minimize->setPalette(pal);
    m_pb_minimize->setFlat(true);
    m_pb_minimize->setStyleSheet("border: 0px;");
    m_pb_minimize->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_minimize_item = scene->addWidget(m_pb_minimize);
    lay_buttons->addItem(pb_minimize_item);

    m_pb_close = new QPushButton();
    m_pb_close->setIcon(QIcon(":/data/icon_close.png"));
    m_pb_close->setIconSize(QSize(button_size * 0.9, button_size * 0.9));
    m_pb_close->setPalette(pal);
    m_pb_close->setFlat(true);
    m_pb_close->setStyleSheet("border: 0px;");
    m_pb_close->setFixedSize(button_size, button_size);
    QGraphicsProxyWidget *pb_close_item = scene->addWidget(m_pb_close);
    lay_buttons->addItem(pb_close_item);

    // creating a widget to contain all the lower buttons
    QGraphicsWidget *buttons_item = new QGraphicsWidget();
    buttons_item->setLayout(lay_buttons);

    scene->addItem(buttons_item);
    buttons_item->setPos((m_screen_width - (button_size + MARGIN) * 7.0 - MARGIN * 2.5) / 2.0, m_screen_height - button_size - 3.0 * MARGIN);

    // connecting lower buttons signals to TimerView slots
    QObject::connect(m_pb_play,      SIGNAL(clicked()), this, SLOT(handlePlayClicked()));
    QObject::connect(m_pb_prev,      SIGNAL(clicked()), this, SLOT(handlePrevClicked()));
    QObject::connect(m_pb_next,      SIGNAL(clicked()), this, SLOT(handleNextClicked()));
    QObject::connect(m_pb_playerout, SIGNAL(clicked()), this, SLOT(handlePlayerOutClicked()));
    QObject::connect(m_pb_rebuy,     SIGNAL(clicked()), this, SLOT(handleRebuyClicked()));
    QObject::connect(m_pb_minimize,  SIGNAL(clicked()), this, SLOT(showMinimized()));
    QObject::connect(m_pb_close,     SIGNAL(clicked()), this, SLOT(close()));

    // creating central texts boxes
    box_name        = QRectF(m_sidebands_width + MARGIN, 0.0, m_screen_width - 2.0 * (m_sidebands_width + MARGIN), 85.0);
    box_timer       = QRectF(m_sidebands_width + MARGIN, box_name.bottom() * 0.8, m_screen_width - 2.0 * (m_sidebands_width + MARGIN), m_screen_height / 3.0);
    box_blinds      = QRectF(m_sidebands_width + MARGIN, box_timer.bottom() * 0.9, m_screen_width - 2.0 * (m_sidebands_width + MARGIN), (m_screen_height - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 4.0);
    box_ante        = QRectF(m_sidebands_width + MARGIN, box_blinds.top() + box_blinds.height() * 0.85, m_screen_width - 2.0 * (m_sidebands_width + MARGIN), (m_screen_height - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 2.5);
    box_next_blinds = QRectF(m_sidebands_width + MARGIN, box_ante.bottom(), m_screen_width - 2.0 * (m_sidebands_width + MARGIN), (m_screen_height - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 2.0);
    box_next_ante   = QRectF(m_sidebands_width + MARGIN, box_next_blinds.top() + box_next_blinds.height() * 0.85, m_screen_width - 2.0 * (m_sidebands_width + MARGIN), (m_screen_height - button_size - 3.0 * MARGIN - box_timer.bottom() * 0.9) / 9.0 * 1.5);

    // creating central variable texts
    m_txt_name = scene->addText(m_tournament->name(), QFont(FONT_NAME));
    m_txt_name->setDefaultTextColor(Qt::white);
    setFontSizeAndCenter(m_txt_name, box_name);

    m_txt_timer = scene->addText("88:88", QFont(FONT_NAME));
    m_txt_timer->setDefaultTextColor(Qt::red);
    setFontSizeAndCenter(m_txt_timer, box_timer);

    m_level_time = m_tournament->currentLevel()->time();
    m_play_time = QTime(0,0,0,0);
    m_txt_timer->setPlainText(m_level_time.toString("mm:ss"));

    m_txt_blinds = scene->addText("", QFont(FONT_NAME));
    m_txt_blinds->setDefaultTextColor(Qt::white);

    m_txt_ante = scene->addText("", QFont(FONT_NAME));
    m_txt_ante->setDefaultTextColor(Qt::white);

    m_txt_next_blinds = scene->addText("400 / 800", QFont(FONT_NAME));
    m_txt_next_blinds->setDefaultTextColor(QColor(0x60,0x60,0x60));

    m_txt_next_ante = scene->addText("Ante: 200", QFont(FONT_NAME));
    m_txt_next_ante->setDefaultTextColor(QColor(0x60,0x60,0x60));

    updateGraphics(LevelsGraphics);

    // setting window properties
    this->setScene(scene);
    this->setSceneRect(0.0, 0.0, m_screen_width, m_screen_height);
    this->setWindowIcon(QIcon(":/data/icon.svg"));
    this->setWindowTitle("Poker Tournament Manager - Torneo");

    // creating and connecting timers
    QTimer *time_timer = new QTimer(this);
    QObject::connect(time_timer, SIGNAL(timeout()), this, SLOT(updateCurrentTime()));
    time_timer->start(1000);

    m_tournament_timer = new QTimer(this);
    QObject::connect(m_tournament_timer, SIGNAL(timeout()), this, SLOT(tournamentTimerTimeout()));
}

TimerView::~TimerView()
{
    delete m_tournament;
}

void TimerView::goToNextLevel()
{
    m_tournament->goToNextLevel();
    m_level_time = m_tournament->currentLevel()->time();
    updateGraphics(LevelsGraphics | PlayTimesGraphics);
}

void TimerView::goToPrevLevel()
{
    m_tournament->goToPrevLevel();
    m_level_time = m_tournament->currentLevel()->time();
    updateGraphics(LevelsGraphics | PlayTimesGraphics);
}

void TimerView::handleNextClicked()
{
    goToNextLevel();

    if(! m_paused)
        m_tournament_timer->start(1000);
}

void TimerView::handlePlayClicked()
{
    if(m_paused)
    {
        if(m_txt_starttime->toPlainText() == "00:00:00" && m_play_time.toString("HH:mm:ss") == "00:00:00")
        {
            m_txt_starttime->setPlainText(QTime::currentTime().toString("HH:mm:ss"));
        }

        m_txt_timer->setDefaultTextColor(Qt::white);
        m_txt_timer->update(m_txt_timer->boundingRect());
        m_tournament_timer->start(1000);
        m_pb_play->setIcon(QIcon(":/data/icon_pause.png"));
        m_paused = false;
    }
    else
    {
        m_paused = true;
        m_pb_play->setIcon(QIcon(":/data/icon_play.png"));
        m_tournament_timer->stop();
        m_txt_timer->setDefaultTextColor(Qt::red);
        m_txt_timer->update(m_txt_timer->boundingRect());
    }
}

void TimerView::handlePlayerOutClicked()
{
    m_tournament->playerOut();
    updateGraphics(AverageStackGraphics | PlayersGraphics | TotalChipsGraphics);
}

void TimerView::handlePrevClicked()
{
    goToPrevLevel();

    if(! m_paused)
        m_tournament_timer->start(1000);
}

void TimerView::handleRebuyClicked()
{
    if(! m_tournament->currentLevel()->isRebuyEnabled())
        return;

    m_tournament->rebuy();
    updateGraphics(AverageStackGraphics | PlayersGraphics | TotalChipsGraphics);
}

void TimerView::tournamentTimerTimeout()
{
    m_level_time = m_level_time.addSecs(-1);

    if(m_level_time.minute() == 0 && m_level_time.second() == 0)
    {
        // cambio livello
        qDebug() << "PLAY GONG";
        PLAYSOUND("sounds/gong.wav");
        goToNextLevel();
    }
    else if(m_level_time.minute() == 1 && m_level_time.second() == 0)
    {
        qDebug() << "PLAY SCREAM";
        PLAYSOUND("sounds/scream.wav");
    }
    else if(m_level_time.minute() == 0 && m_level_time.second() >= 1 && m_level_time.second() <= 3)
    {
        qDebug() << "PLAY TICK";
        PLAYSOUND("sounds/tick.wav");
    }

    m_play_time = m_play_time.addSecs(1);
    updateGraphics(PlayTimesGraphics);
}

void TimerView::updateCurrentTime()
{
    updateGraphics(CurrentTimeGraphics);
}

void TimerView::updateGraphics(int what)
{
    if(what & AverageStackGraphics)
    {
        m_txt_avgstack->setPlainText(addNumberPoints(QString::number(m_tournament->averageStack())));
        setFontSizeAndCenter(m_txt_avgstack, box_2);
    }

    if(what & CurrentTimeGraphics)
    {
        m_txt_time->setPlainText(QTime::currentTime().toString("HH:mm:ss"));
    }

    if(what & LevelsGraphics)
    {
        if(m_tournament->currentLevel()->type() == Level::GameLevel)
        {
            m_txt_blinds->setPlainText(QString("%1 / %2").arg(m_tournament->currentLevel()->smallBlind()).arg(m_tournament->currentLevel()->bigBlind()));
            setFontSizeAndCenter(m_txt_blinds, box_blinds);

            m_txt_ante->setPlainText(QString("Ante: %1").arg(m_tournament->currentLevel()->ante()));
            setFontSizeAndCenter(m_txt_ante, box_ante);

            m_pb_rebuy->setIcon(QIcon(QString(":/data/icon_rebuy%1.png").arg(m_tournament->currentLevel()->isRebuyEnabled() ? "" : "_locked")));
        }
        else
        {
            m_txt_blinds->setPlainText(tr("PAUSA"));
            setFontSizeAndCenter(m_txt_blinds, box_blinds);

            m_txt_ante->setPlainText("RELAX");
            setFontSizeAndCenter(m_txt_ante, box_ante);

            m_pb_rebuy->setIcon(QIcon(":/data/icon_rebuy_locked.png"));
        }

        if(m_tournament->nextLevel()->type() == Level::GameLevel)
        {
            m_txt_next_blinds->setPlainText(QString("%1 / %2").arg(m_tournament->nextLevel()->smallBlind()).arg(m_tournament->nextLevel()->bigBlind()));
            setFontSizeAndCenter(m_txt_next_blinds, box_next_blinds);

            m_txt_next_ante->setPlainText(QString("Ante: %1").arg(m_tournament->nextLevel()->ante()));
            setFontSizeAndCenter(m_txt_next_ante, box_next_ante);
        }
        else
        {
            m_txt_next_blinds->setPlainText(tr("PAUSA"));
            setFontSizeAndCenter(m_txt_next_blinds, box_next_blinds);

            m_txt_next_ante->setPlainText("RELAX");
            setFontSizeAndCenter(m_txt_next_ante, box_next_ante);
        }
    }

    if(what & PlayersGraphics)
    {
        m_txt_players->setPlainText(QString("%1/%2").arg(m_tournament->currentPlayers()).arg(m_tournament->totalPlayers()));
        setFontSizeAndCenter(m_txt_players, box_1);
    }

    if(what & PlayTimesGraphics)
    {
        m_txt_timer->setPlainText(m_level_time.toString("mm:ss"));
        m_txt_playtime->setPlainText(m_play_time.toString("HH:mm:ss"));
    }

    if(what & TotalChipsGraphics)
    {
        m_txt_totchips->setPlainText(addNumberPoints(QString::number(m_tournament->totalChips())));
        setFontSizeAndCenter(m_txt_totchips, box_3);
    }
}
