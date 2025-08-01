#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include<vector>
#include<QString>
#include"player.h"

class GameManager
{
public:
    std::vector<Player*> m_playerList;
    GameManager();
    QString getInfo()const;
    int getPlayerCount()const;
    int getTourCount()const;
    void setPlayerCount(const int playerCount);
    void setTourCount(const int tourCount);
    void setInfo(const QString Info);
    QString getTourName()const;
    QString getDate()const;
    void setTourName(const QString tourName);
    void setDate(const QString date);
    void addNewPlayer(Player* P);
private:
    int m_playerCount;
    int m_tourCount;
    QString m_tourName;
    QString m_date;
    QString m_info;
};

#endif // GAMEMANAGER_H
