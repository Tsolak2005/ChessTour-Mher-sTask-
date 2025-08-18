#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<vector>
#include<QString>
#include"player.h"
#include "game.h"

class GameManager
{
public:

    GameManager();
    QString getInfo()const;
    int getPlayerCount()const;
    int getTourCount()const;
    int getIndexOfTournament()const;
    int getCurrentTour()const;
    QString getTourName()const;
    QString getDate()const;
    std::vector<Game *> *getTourGames(int tour);
    int getSizeOfGameMap() const;
    Player * getPlayerById(const int id) const;
    void setIndexOfTournament(const int index);
    void setPlayerCount(const int playerCount);
    void setTourCount(const int tourCount);
    void setInfo(const QString Info);
    void setCurrentTour(const int tour);
    void setGame(int tour, Game* game);

    void setTourName(const QString tourName);
    void setDate(const QString date);
    void addNewPlayer(Player* P);
    bool hasTheTournamentStarted();

private:
    int m_IndexOfTournament;
    int m_playerCount;
    int m_tourCount;
    int m_currentTour;
    QString m_tourName;
    QString m_date;
    QString m_info;

    std::map< int, std::vector<Game*> > m_gameMap;
    std::vector<Player*> m_playerList;
};

#endif // GAMEMANAGER_H
