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

#include "Tournament.h"

#include <QDebug>

#define MIN_GAMELEVEL_MINUTES  2
#define MIN_PAUSELEVEL_MINUTES 1

// ===== CLASS "Level" ============================================================================

Level::Level()
{
    m_time.setHMS(0,0,0);

    m_ante       = 0;
    m_bigblind   = 0;
    m_smallblind = 0;

    m_rebuy_enabled = false;
    m_type = GameLevel;
}

Level::~Level()
{
}

int Level::ante()
{
    return m_ante;
}

int Level::bigBlind()
{
    return m_bigblind;
}

bool Level::isRebuyEnabled()
{
    return m_rebuy_enabled;
}

bool Level::isValid()
{
    if(type() == GameLevel)
    {
        // small blind and big blind must have a positive non zero value
        if(smallBlind() == 0 || bigBlind() == 0)
            return false;

        // a game level must be at least two minutes long
        if(time().minute() < MIN_GAMELEVEL_MINUTES)
            return false;

        return true;
    }

    if(type() == PauseLevel)
    {
        // a pause level must be at least one minute long
        if(time().minute() < MIN_PAUSELEVEL_MINUTES)
            return false;

        return true;
    }

    return false;
}

void Level::setAnte(int a)
{
    if(a < 0)
        return;

    m_ante = a;
}

void Level::setBigBlind(int bb)
{
    if(bb < 0)
        return;

    m_bigblind = bb;
}

void Level::setRebuyEnabled(bool r)
{
    m_rebuy_enabled = r;
}

void Level::setSmallBlind(int sb)
{
    if(sb < 0)
        return;

    m_smallblind = sb;
}

void Level::setTime(const QTime& t)
{
    setTime(t.minute(), t.second());
}

void Level::setTime(int min, int sec)
{
    m_time = QTime(0, min, sec);
}

void Level::setType(LevelType t)
{
    m_type = t;
}

int Level::smallBlind()
{
    return m_smallblind;
}

QString Level::strType()
{
    switch(type())
    {
        case GameLevel:
            return tr("Gioco");

        case PauseLevel:
            return tr("Pausa");
    }

    return tr("Sconosciuto");
}

QTime Level::time()
{
    return m_time;
}

QString Level::toString()
{
    QString strtype = "U";

    switch(type())
    {
        case GameLevel:
            strtype = "G";
            break;

        case PauseLevel:
            strtype = "P";
            break;
    }

    return QString("[Type:%1, Time:%2, Rebuy:%6, Ante:%3, SB:%4, BB:%5]").arg(strtype).arg(time().toString("mm:ss")).arg(ante()).arg(smallBlind()).arg(bigBlind()).arg(isRebuyEnabled()? "Y":"N");
}

Level::LevelType Level::type()
{
    return m_type;
}

// ===== CLASS "Tournament" =======================================================================

Tournament::Tournament()
{
    m_chips_each      = 0;
    m_current_players = 0;
    m_rebuy_maxlevel  = 0;
    m_rebuy_chips     = 0;
    m_rebuys          = 0;
    m_total_players   = 0;
}

Tournament::~Tournament()
{
//    qDeleteAll(m_levels);
    foreach(Level *l, m_levels)
        delete l;
}

Level* Tournament::addLevel(int pos)
{
    Level* l = new Level;

    if(pos < 0 || pos >= m_levels.size())
        m_levels.append(l);
    else
        m_levels.insert(pos, l);

    return l;
}

int Tournament::averageStack()
{
    return totalChips() / m_current_players;
}

int Tournament::chipsEach()
{
    return m_chips_each;
}

int Tournament::currentPlayers()
{
    return m_current_players;
}

Level* Tournament::level(int number)
{
    //qDebug() << this->toString();

    if(number < 0 || countLevels() == 0)
        return 0;

    return m_levels.at(qMin(number, countLevels() - 1));
}

int Tournament::countLevels()
{
    return m_levels.size();
}

void Tournament::moveLevelDown(int pos)
{
    if(pos < 0 || pos >= m_levels.size() - 1)
        return;

    m_levels.swap(pos,pos+1);
}

void Tournament::moveLevelUp(int pos)
{
    if(pos <= 0 || pos > m_levels.size() - 1)
        return;

    m_levels.swap(pos,pos-1);
}

QString Tournament::name()
{
    return m_name;
}

int Tournament::rebuyChips()
{
    return m_rebuy_chips;
}

int Tournament::rebuyMaxLevel()
{
    return m_rebuy_maxlevel;
}

void Tournament::removeLevel(int pos)
{
    if(pos < 0 || pos >= m_levels.size() - 1)
    {
        Level *l = m_levels.takeLast();
        delete l;
    }
    else
    {
        Level *l = m_levels.takeAt(pos);
        delete l;
    }
}

int Tournament::totalChips()
{
    return m_total_players * m_chips_each + m_rebuys * m_rebuy_chips;
}

int Tournament::totalPlayers()
{
    return m_total_players;
}

void Tournament::playerOut()

{
    if(m_current_players > 1)
        m_current_players--;
}

void Tournament::rebuy(int currentLevel)
{
    if(currentLevel >= m_rebuy_maxlevel || m_current_players == m_total_players)
        return;

    m_current_players++;
    m_rebuys++;
}

void Tournament::setChipsEach(int c)
{
    m_chips_each = c;
}

void Tournament::setCurrentPlayers(int cp)
{
    m_current_players = cp;
}

void Tournament::setName(const QString& n)
{
    m_name = n;
}

void Tournament::setTotalPlayers(int tp)
{
    m_total_players = tp;
}

void Tournament::setRebuyChips(int c)
{
    m_rebuy_chips = c;
}

void Tournament::setRebuyMaxLevel(int l)
{
    m_rebuy_maxlevel = l;
}

QString Tournament::toString()
{
    QString str;
    foreach(Level* l, m_levels)
    {
        str.append(QString("%1 ").arg(l->toString()));
    }
    return str;
}
