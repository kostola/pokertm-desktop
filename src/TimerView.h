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

#ifndef TIMERVIEW_H
#define TIMERVIEW_H

#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QObject>
#include <QPushButton>
#include <QRectF>
#include <QTime>
#include <QTimer>

#include "Tournament.h"

class TimerView : public QGraphicsView
{
    Q_OBJECT

    public:
        TimerView(Tournament *t);

    private:
        void goToNextLevel();
        void goToPrevLevel();

        Tournament *m_tournament;

        bool m_paused;
        int m_current_level;

        double m_screen_width;
        double m_screen_height;
        double m_sidebands_width;

        QTime m_level_time;
        QTime m_play_time;

        QTimer *m_tournament_timer;

        QRectF box_1;
        QRectF box_2;
        QRectF box_3;
        QRectF box_4;
        QRectF box_5;
        QRectF box_6;
        QRectF box_name;
        QRectF box_timer;
        QRectF box_blinds;
        QRectF box_ante;
        QRectF box_next_blinds;
        QRectF box_next_ante;

        QGraphicsTextItem *m_txt_avgstack;
        QGraphicsTextItem *m_txt_name;
        QGraphicsTextItem *m_txt_players;
        QGraphicsTextItem *m_txt_playtime;
        QGraphicsTextItem *m_txt_starttime;
        QGraphicsTextItem *m_txt_time;
        QGraphicsTextItem *m_txt_timer;
        QGraphicsTextItem *m_txt_totchips;
        QGraphicsTextItem *m_txt_blinds;
        QGraphicsTextItem *m_txt_ante;
        QGraphicsTextItem *m_txt_next_blinds;
        QGraphicsTextItem *m_txt_next_ante;

        QPushButton *m_pb_close;
        QPushButton *m_pb_minimize;
        QPushButton *m_pb_next;
        QPushButton *m_pb_play;
        QPushButton *m_pb_playerout;
        QPushButton *m_pb_prev;
        QPushButton *m_pb_rebuy;

    private slots:
        void handleNextClicked();
        void handlePlayClicked();
        void handlePlayerOutClicked();
        void handlePrevClicked();
        void handleRebuyClicked();
        void tournamentTimerTimeout();
        void updateAverageStack();
        void updateCurrentTime();
        void updateLevels();
        void updatePlayers();
        void updatePlayTimes();
        void updateTotalChips();
};

#endif // TIMERVIEW_H
