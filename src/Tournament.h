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
#include <QTime>

class Level
{
    public:
        enum LevelType
        {
            GameLevel,
            PauseLevel
        };

        int ante();
        void setAnte(int a);

        int bigBlind();
        void setBigBlind(int bb);

        int smallBlind();
        void setSmallBlind(int sb);

        bool isRebuyEnabled();
        void setRebuyEnabled(bool r);

        QTime time();
        void setTime(const QTime& t);
        void setTime(int min, int sec = 0);

        LevelType type();
        void setType(LevelType t);

        bool isValid();
        QString toString();

    private:
        Level();
        ~Level();

        friend class Tournament;

        int m_ante;
        int m_bigblind;
        int m_smallblind;

        bool m_rebuy_enabled;
        QTime m_time;
        LevelType m_type;
};

class Tournament
{
    public:
        Tournament();
        ~Tournament();

        Level* addLevel();
        Level* level(int number);
        int countLevels();

        int chipsEach();
        void setChipsEach(int c);

        int currentPlayers();
        void setCurrentPlayers(int cp);

        QString name();
        void setName(const QString& n);

        int rebuyChips();
        void setRebuyChips(int c);

        int rebuyMaxLevel();
        void setRebuyMaxLevel(int l);

        int totalPlayers();
        void setTotalPlayers(int tp);

        int averageStack();
        int totalChips();

        void playerOut();
        void rebuy(int currentLevel);

        QString toString();

    private:
        QString m_name;
        QList<Level*> m_levels;
        int m_chips_each;
        int m_current_players;
        int m_rebuy_maxlevel;
        int m_rebuy_chips;
        int m_rebuys;
        int m_total_players;
};

#endif // TOURNAMENT_H
