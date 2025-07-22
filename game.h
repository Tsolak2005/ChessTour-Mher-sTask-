#ifndef GAME_H
#define GAME_H

class Game
{
private:
    int white_player_id;
    int black_player_id;
    int result;

public:
    Game(int w, int b, int res = -2);

    int GetWhite_player_id() const;
    int GetBlack_player_id() const;
    int GetResult() const;

    void SetWhite_player_id(int);
    void SetBlack_player_id(int);
    void SetResult(int);

    bool isResultDefined() const;

};

#endif // GAME_H
