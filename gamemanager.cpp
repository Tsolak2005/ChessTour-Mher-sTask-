#include "gamemanager.h"

GameManager::GameManager() : m_playerCount(0), m_tourCount(0) {}

int GameManager::getPlayerCount() const
{
    return m_playerCount;
}

int GameManager::getTourCount() const
{
    return m_tourCount;
}

void GameManager::setPlayerCount(const int playerCount)
{
    m_playerCount = playerCount;
}

void GameManager::setTourCount(const int tourCount)
{
    m_tourCount = tourCount;
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
