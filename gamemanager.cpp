#include "gamemanager.h"

GameManager::GameManager() : m_currentoOganizedTour(1), m_playerCount(0), m_tourCount(0), m_currentTour(1) {}

int GameManager::getCurrentoOganizedTour() const
{
    return m_currentoOganizedTour;
}

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

int GameManager::getIndexOfTournament() const
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

void GameManager::setRadioButtonsOfTabel(QRadioButton *radioButton)
{
    m_vecotrOfRadioButtonsOfTabel.push_back(radioButton);
}

void GameManager::setCurrentoOganizedTour(const int tour)
{
    m_currentoOganizedTour = tour;
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

std::vector<Game*>* GameManager::getTourGames(int tour)
{

    if(tour <= m_tourCount && tour > 0)
    {
        return &m_gameMap[tour];
    }
    return nullptr;
}

std::vector<QRadioButton *>* GameManager::getRadioButtonsOfTabel()
{
    return &m_vecotrOfRadioButtonsOfTabel;
}

int GameManager::getSizeOfGameMap() const
{
    return m_gameMap.size();
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

bool GameManager::hasTheTournamentStarted()
{
    return (m_gameMap.size() > 1) || ((*getTourGames(m_currentTour))[0]->getResult() != -2);
}
