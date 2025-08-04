#include "gamemanager.h"

GameManager::GameManager() : m_playerCount(0), m_tourCount(0) {}

QString GameManager::getInfo()const
{
    return m_info;
}

int GameManager::getPlayerCount() const
{
    return m_playerCount;
}

int GameManager::getTourCount() const
{
    return m_tourCount;
}


Player * GameManager::getPlayerById(const int id) const
{
    for (int i = 0; i < m_playerCount; ++i) {
        if (m_playerList[i]->getId() == id) {
            return m_playerList[i];
        }
    }
    throw std::runtime_error("There is not  player with such Id");
}

void GameManager::setPlayerCount(const int playerCount)
{
    m_playerCount = playerCount;
}

void GameManager::setTourCount(const int tourCount)
{
    m_tourCount = tourCount;
}

void GameManager::setInfo(const QString Info)
{
    m_info = Info;
}

QString GameManager::getTourName() const
{
    return m_tourName;
}

QString GameManager::getDate() const
{
    return m_date;
}

void GameManager::setTourName(const QString tourName)
{
    m_tourName = tourName;
}

void GameManager::setDate(const QString date)
{
    m_date = date;
}

void GameManager::addNewPlayer(Player* P)
{
    m_playerList.push_back(P);
}
