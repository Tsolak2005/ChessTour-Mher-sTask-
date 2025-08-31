#include "game.h"

Game::Game(int w, int b, int res) : m_whitePlayerId(w), m_blackPlayerId(b), m_result(res) {}

Game::Game(int w): m_whitePlayerId(w), m_result(-2) {}

Game::Game(Game & w): m_whitePlayerId(w.m_whitePlayerId), m_blackPlayerId(w.m_blackPlayerId), m_result(w.m_result)
{

}


int Game::getWhitePlayerId() const {
    return m_whitePlayerId;
}
int Game::getBlackPlayerId() const {
    return m_blackPlayerId;
}
int Game::getResult() const {
    return m_result;
}

void Game::setWhitePlayerId(int id) {
    m_whitePlayerId = id;
}
void Game::setBlackPlayerId(int id) {
    m_blackPlayerId = id;
}
void Game::setResult(int res) {
    m_result = res;
}

bool Game::isResultDefined() const {
    return m_result != -2;
}
