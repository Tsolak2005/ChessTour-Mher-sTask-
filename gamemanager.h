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
    int getIndexOfTournamnet()const;
    int getCurrentTour()const;
    Player * getPlayerById(const int id) const;
    void setIndexOfTournament(const int index);
    void setPlayerCount(const int playerCount);
    void setTourCount(const int tourCount);
    void setInfo(const QString Info);
    void setCurrentTour(const int tour);
    QString getTourName()const;
    QString getDate()const;
    void setTourName(const QString tourName);
    void setDate(const QString date);
    void addNewPlayer(Player* P);
    bool isTheTournamnetStarted();

private:
    int m_IndexOfTournament;
    int m_playerCount;
    int m_tourCount;
    int m_currentTour;
    QString m_tourName;
    QString m_date;
    QString m_info;

    std::vector<std::vector<Game*>> m_gameList;
    std::vector<Player*> m_playerList;
};

#endif // GAMEMANAGER_H
