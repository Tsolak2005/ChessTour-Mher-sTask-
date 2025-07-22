#include "game.h"

Game::Game(int w, int b, int res) : white_player_id(w), black_player_id(b), result(res) {}

int Game::GetWhite_player_id() const { return white_player_id; }
int Game::GetBlack_player_id() const { return black_player_id; }
int Game::GetResult() const { return result; }

void Game::SetWhite_player_id(int id) { white_player_id = id; }
void Game::SetBlack_player_id(int id) { black_player_id = id; }
void Game::SetResult(int res) { result = res; }

bool Game::isResultDefined() const { return result != -2; }
