#include "gamemanager.h"

GameManager::GameManager() : m_playerCount(0), m_tourCount(0), m_currentTour(1) {}

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

int GameManager::getIndexOfTournamnet() const
{
    return m_IndexOfTournament;
}

int GameManager::getCurrentTour() const
{
    return m_currentTour;
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

void GameManager::setIndexOfTournament(const int index)
{
    m_IndexOfTournament = index;
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

void GameManager::setCurrentTour(const int tour)
{
    m_currentTour = tour;
}

void GameManager::setGame(int tour, Game* game)
{
    if(tour <= m_tourCount && tour > 0)
    {
        m_gameMap[tour].push_back(game);
    }
}

QString GameManager::getTourName() const
{
    return m_tourName;
}

QString GameManager::getDate() const
{
    return m_date;
}

const std::vector<Game*>* GameManager::getTourgames(int tour) const
{

    if(tour <= m_tourCount && tour > 0)
    {
        auto it = m_gameMap.find(tour);
        // if (it != m_gameMap.end()) {
        //     return &it->second;
        // }
    }
    return nullptr;
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

bool GameManager::isTheTournamnetStarted()
{
    return !m_gameMap.empty();
}
