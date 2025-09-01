#include "gamemanager.h"

GameManager::GameManager(int playerCount) : m_currentoOganizedTour(1), m_playerCount(0),
    m_tourCount(0), m_currentTour(1), m_vectorOfRadioButtonsOfTabel(new std::vector<QRadioButton*>),
    m_matrixOfPlayers(new std::vector<std::vector<bool>>(playerCount, std::vector<bool>(playerCount, false)))
{
    for(int i=0; i<m_playerCount; ++i)
    {
        (*m_matrixOfPlayers)[i][i] = true;
    }
}

GameManager::GameManager(GameManager* parent)
    : m_IndexOfTournament(parent->m_IndexOfTournament),
    m_currentoOganizedTour(parent->m_currentoOganizedTour),
    m_playerCount(parent->m_playerCount),
    m_tourCount(parent->m_tourCount),
    m_currentTour(parent->m_currentTour),
    m_tourName(parent->m_tourName),
    m_date(parent->m_date),
    m_info(parent->m_info),
    m_vectorOfRadioButtonsOfTabel(new std::vector<QRadioButton*>())
{
    // Deep copy radio buttons
    for (auto* btn : *parent->m_vectorOfRadioButtonsOfTabel) {
        QRadioButton* newBtn = new QRadioButton(btn->text());
        newBtn->setChecked(btn->isChecked());
        m_vectorOfRadioButtonsOfTabel->push_back(newBtn);
    }

    // Deep copy players (requires Player::clone())
    for (auto* it : parent->m_playerList) {
        m_playerList.push_back(new Player(it));
    }

    // Deep copy games (requires Game::clone())
    for (const auto& [id, games] : parent->m_gameMap) {
        std::vector<Game*> copies;
        for (auto* g : games) {
            copies.push_back(new Game(*g));
        }
        m_gameMap[id] = std::move(copies);
    }

    delete m_matrixOfPlayers;
}

GameManager::~GameManager()
{
    // 1. Delete all games stored in m_gameMap
    for (auto& pair : m_gameMap) {
        for (auto* game : pair.second) {
            delete game;
        }
    }
    m_gameMap.clear();

    // 2. Delete all players
    for (auto* player : m_playerList) {
        delete player;
    }
    m_playerList.clear();

    // 3. Delete all radio buttons of the table
    if (m_vectorOfRadioButtonsOfTabel) {
        for (auto* rb : *m_vectorOfRadioButtonsOfTabel) {
            delete rb;  // careful: only if you created them without a parent!
        }
        delete m_vectorOfRadioButtonsOfTabel; // since it's a pointer to vector
        m_vectorOfRadioButtonsOfTabel = nullptr;
    }

}

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
    m_vectorOfRadioButtonsOfTabel->push_back(radioButton);
}

void GameManager::setCurrentOganizedTour(const int tour)
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

void GameManager::changePlayersList(std::vector<Player *> *P)
{
    m_playerList.clear();
    for(auto * it: *P)
    {
        m_playerList.push_back(new Player(it));
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
    return m_vectorOfRadioButtonsOfTabel;
}

std::vector<Player *>* GameManager::getPlayers()
{
    return &m_playerList;
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

bool GameManager::HaveThePlayersMet(int p1Id, int p2Id)
{
    return (*m_matrixOfPlayers)[p1Id-1][p2Id-1];
}

void GameManager::ThePlayerSMet(int p1Id, int p2Id)
{
    (*m_matrixOfPlayers)[p1Id-1][p2Id-1] = true;
    (*m_matrixOfPlayers)[p2Id-1][p1Id-1] = true;
}

void GameManager::changeMatrixOfPlayers(int playerCount, int lastPlayerCount)
{
    if(playerCount<m_playerCount) qDebug() << "inserted players count is less then the existing";

    std::vector<std::vector<bool>>* temp = new std::vector<std::vector<bool>>(playerCount, std::vector<bool>(playerCount, false));

    for(int i=0; i<playerCount; ++i)
    {
        (*temp)[i][i] = true;
    }
    std::cout << "ERROR BEFORE FOR"<<std::endl;
    for(int i=0; i<lastPlayerCount; ++i)
    {
        for(int j=0; j<lastPlayerCount; ++j)
        {
            (*temp)[i][j] = (*m_matrixOfPlayers)[i][j];
        }
    }
    std::cout << "ERROR AFTDER FOR"<<std::endl;
    for(int i=0; i<lastPlayerCount; ++i)
    {
        for(int j=0; j<lastPlayerCount; ++j)
        {
            std::cout << (*temp)[j][i]<< " ";
        }
        std::cout << std::endl;
    }

    delete m_matrixOfPlayers;
    m_matrixOfPlayers = temp;
    temp = nullptr;
}


