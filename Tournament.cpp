#include "Tournament.h"

Tournament::Tournament(QObject *parent)
    : QObject(parent)
{
}

void Tournament::addLevel(Level l)
{
    m_levels.append(l);
}

int Tournament::getChipsEach()
{
    return m_chips_each;
}

Tournament::Level Tournament::getLevel(int number)
{
    if(number >= getLevels())
        return Level();

    return m_levels.at(number);
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

int Tournament::getTotalPlayers()
{
    return m_players;
}

void Tournament::setChipsEach(int c)
{
    m_chips_each = c;
}

void Tournament::setName(const QString& n)
{
    m_name = n;
}

void Tournament::setTotalPlayers(int tp)
{
    m_players = tp;
}

void Tournament::setRebuyChips(int c)
{
    m_rebuy_chips = c;
}

void Tournament::setRebuyMaxLevel(int l)
{
    m_rebuy_maxlevel = l;
}
