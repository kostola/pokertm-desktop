#include "Tournament.h"

#include <QDebug>

Tournament::Tournament(QObject *parent)
    : QObject(parent)
{
    m_chips_each = 0;
    m_current_players = 0;
    m_rebuy_maxlevel = 0;
    m_rebuy_chips = 0;
    m_rebuys = 0;
    m_total_players = 0;
}

void Tournament::addLevel(Level l)
{
    m_levels.append(l);
}

int Tournament::getAverageStack()
{
    return getTotalChips() / m_current_players;
}

int Tournament::getChipsEach()
{
    return m_chips_each;
}

int Tournament::getCurrentPlayers()
{
    return m_current_players;
}

Tournament::Level Tournament::getLevel(int number)
{
    //qDebug() << this->toString();

    if(number < 0 || getLevels() == 0)
        return Level();

    return m_levels.at(std::min(number, getLevels() - 1));
}

int Tournament::getLevels()
{
    return m_levels.size();
}

QString Tournament::getName()
{
    return m_name;
}

int Tournament::getRebuyChips()
{
    return m_rebuy_chips;
}

int Tournament::getRebuyMaxLevel()
{
    return m_rebuy_maxlevel;
}

int Tournament::getTotalChips()
{
    return m_total_players * m_chips_each + m_rebuys * m_rebuy_chips;
}

int Tournament::getTotalPlayers()
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
    foreach(Level l, m_levels) {
        str.append(QString("[Tempo:%1, Ante:%2, SB:%3, BB:%4] ").arg(l.time_minutes).arg(l.ante).arg(l.smallblind).arg(l.bigblind));
    }
    return str;
}
