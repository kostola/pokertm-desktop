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

#include <QDomDocument>
#include <QList>
#include <QObject>
#include <QString>
#include <QTime>

class Level : public QObject
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
        QString strType();
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

class Tournament : public QObject
{
    public:
        Tournament();
        ~Tournament();

        // configuration methods
        Level* addLevel(int pos = -1);
        int countLevels();
        Level* level(int number);
        void moveLevelDown(int pos);
        void moveLevelUp(int pos);
        void removeLevel(int pos = -1);

        int initialStack();
        void setInitialStack(int c);

        QString name();
        void setName(const QString& n);

        int rebuyStack();
        void setRebuyStack(int c);

        int totalPlayers();
        void setTotalPlayers(int tp);

        bool isStarted();
        void start();

        // play methods
        Level* currentLevel();
        int currentLevelNumber();
        void goToNextLevel();
        void goToPrevLevel();
        Level* nextLevel();

        int averageStack();
        int currentPlayers();
        int totalChips();

        void playerOut();
        void rebuy();

        // other methods
        QString toString();

        bool fromXml(const QDomDocument& xmldoc);
        void toXml(QDomDocument& xmldoc);

    private:
        QString m_name;
        QList<Level*> m_levels;
        int m_stack_initial;
        int m_stack_rebuy;
        int m_total_players;

        bool m_is_started;
        int m_current_level;
        int m_current_players;
        int m_rebuys;

};

#endif // TOURNAMENT_H
