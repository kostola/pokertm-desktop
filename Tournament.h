#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QList>
#include <QObject>
#include <QString>

class Tournament : public QObject
{
    public:
        typedef struct {
            int ante;
            int bigblind;
            int smallblind;
        } Level;

        Tournament(QObject *parent = 0);
        void addLevel(Level l);
        int getChipsEach();
        Level getLevel(int number);
        int getLevels();
        QString getName();
        int getRebuyChips();
        int getRebuyMaxLevel();
        int getTotalPlayers();
        void setChipsEach(int c);
        void setName(const QString& n);
        void setTotalPlayers(int tp);
        void setRebuyChips(int c);
        void setRebuyMaxLevel(int l);

    private:
        QString m_name;
        QList<Level> m_levels;
        int m_players;
        int m_chips_each;
        int m_rebuy_maxlevel;
        int m_rebuy_chips;
};

#endif // TOURNAMENT_H
