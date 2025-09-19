#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<vector>
#include<QString>
#include"player.h"
#include "game.h"
#include <QRadioButton>
#include <iostream>

class GameManager
{
public:

    GameManager(int playerCount);
    ~GameManager();

    int getCurrentoOganizedTour()const;
    void setCurrentOganizedTour(const int tour);

    QString getInfo()const;
    void setInfo(const QString Info);

    int getPlayerCount()const;
    void setPlayerCount(const int playerCount);

    int getTourCount()const;
    void setTourCount(const int tourCount);

    int getIndexOfTournament()const;
    void setIndexOfTournament(const int index);

    int getCurrentTour()const;
    void setCurrentTour(const int tour);

    QString getTourName()const;

    QString getDate()const;

    std::vector<Game *> *getTourGames(int tour);
    void setGame(int tour, Game* game);
    int getSizeOfGameMap() const;

    std::vector<Player*>*getPlayers();
    void changePlayersList(std::vector<Player*>*);
    Player * getPlayerById(const int id) const;
    void addNewPlayer(Player* P);

    void setTourName(const QString tourName);
    void setDate(const QString date);

    bool hasTheTournamentStarted();

    bool HaveThePlayersMet(int p1Id, int p2Id);
    void ThePlayerSMet(int p1Id, int p2ID);
    void changeMatrixOfPlayers(int playerCount, int lastPlayerCount);


private:

    int m_IndexOfTournament;
    int m_currentoOganizedTour;
    int m_playerCount;
    int m_tourCount;
    int m_currentTour;
    QString m_tourName;
    QString m_date;
    QString m_info;

    std::map< int, std::vector<Game*> > m_gameMap;
    std::vector<Player*> m_playerList;
    std::vector<std::vector<bool>>* m_matrixOfPlayers;
};

#endif // GAMEMANAGER_H
