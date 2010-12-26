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

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QList>
#include <QObject>
#include <QString>

class Tournament : public QObject
{
    public:
        typedef struct {
            int time_minutes;
            int ante;
            int bigblind;
            int smallblind;
        } Level;

        Tournament(QObject *parent = 0);
        void addLevel(Level l);
        int getAverageStack();
        int getChipsEach();
        int getCurrentPlayers();
        Level getLevel(int number);
        int getLevels();
        QString getName();
        int getRebuyChips();
        int getRebuyMaxLevel();
        int getTotalChips();
        int getTotalPlayers();
        void playerOut();
        void rebuy(int currentLevel);
        void setChipsEach(int c);
        void setCurrentPlayers(int cp);
        void setName(const QString& n);
        void setTotalPlayers(int tp);
        void setRebuyChips(int c);
        void setRebuyMaxLevel(int l);
        QString toString();

    private:
        QString m_name;
        QList<Level> m_levels;
        int m_chips_each;
        int m_current_players;
        int m_rebuy_maxlevel;
        int m_rebuy_chips;
        int m_rebuys;
        int m_total_players;
};

#endif // TOURNAMENT_H
