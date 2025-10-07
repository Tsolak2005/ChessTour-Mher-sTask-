 #include "gamemanager.h"

GameManager::GameManager(int playerCount) : m_currentOrganizedTour(1), m_playerCount(0),
    m_tourCount(0), m_currentTour(1),
    m_matrixOfPlayers(playerCount, std::vector<bool>(playerCount, false))
{
    for(int i=0; i<m_playerCount; ++i)
    {
        m_matrixOfPlayers[i][i] = true;
    }
}

GameManager::~GameManager()
{
    // // 1. Delete all games stored in m_gameMap
    // for (auto& pair : m_gameMap) {
    //     for (auto* game : pair.second) {
    //         delete game;
    //     }
    // }
    // m_gameMap.clear();

    // // 2. Delete all players
    // for (auto* player : m_playerList) {
    //     delete player;
    // }
    // m_playerList.clear();
}

int GameManager::getCurrentoOganizedTour() const
{
    return m_currentOrganizedTour;
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


std::shared_ptr<Player>& GameManager::getPlayerById(const int id)
{
    for (int i = 0; i < m_playerCount; ++i) {
        if (m_playerList[i]->getId() == id) {
            return m_playerList[i];
        }
    }
    throw std::runtime_error("There is not  player with such Id");
}


void GameManager::setCurrentOganizedTour(const int tour)
{
    m_currentOrganizedTour = tour;
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

void GameManager::setGame(int tour, std::shared_ptr<Game> game)
{
    if(tour <= m_tourCount && tour > 0)
    {
        m_gameMap[tour].push_back(game);
    }
}

void GameManager::changePlayersList(const std::vector<std::shared_ptr<Player>>& P)
{
    m_playerList = P;
}

QString GameManager::getTourName() const
{
    return m_tourName;
}

QString GameManager::getDate() const
{
    return m_date;
}

const std::vector<std::shared_ptr<Game>>& GameManager::getTourGames(int tour) const
{

    if(tour <= m_tourCount && tour > 0)
    {
        return m_gameMap.at(tour);
    }

    throw std::out_of_range("Invalid tour number");
}

std::vector<std::shared_ptr<Player>>& GameManager::getPlayers()
{
    return m_playerList;
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

void GameManager::addNewPlayer(const std::shared_ptr<Player>& P)
{
    m_playerList.push_back(P);
}

bool GameManager::hasTheTournamentStarted()
{
    return (m_gameMap.size() > 1) || (getTourGames(m_currentTour)[0]->getResult() != -2);
}

bool GameManager::HaveThePlayersMet(int p1Id, int p2Id)
{
    return m_matrixOfPlayers[p1Id-1][p2Id-1];
}

void GameManager::ThePlayerSMet(int p1Id, int p2Id)
{
    m_matrixOfPlayers[p1Id-1][p2Id-1] = true;
    m_matrixOfPlayers[p2Id-1][p1Id-1] = true;
}

void GameManager::changeMatrixOfPlayers(int playerCount, int lastPlayerCount)
{
    if(playerCount<m_playerCount) qDebug() << "inserted players count is less then the existing";

    std::vector<std::vector<bool>> temp(playerCount, std::vector<bool>(playerCount, false));

    for(int i=0; i<playerCount; ++i)
    {
        temp[i][i] = true;
    }
    std::cout << "ERROR BEFORE FOR"<<std::endl;
    for(int i=0; i<lastPlayerCount; ++i)
    {
        for(int j=0; j<lastPlayerCount; ++j)
        {
            temp[i][j] = m_matrixOfPlayers[i][j];
        }
    }
    std::cout << "ERROR AFTER FOR"<<std::endl;
    for(int i=0; i<lastPlayerCount; ++i)
    {
        for(int j=0; j<lastPlayerCount; ++j)
        {
            std::cout << temp[j][i]<< " ";
        }
        std::cout << std::endl;
    }

    m_matrixOfPlayers.clear();
    m_matrixOfPlayers = temp;
}
