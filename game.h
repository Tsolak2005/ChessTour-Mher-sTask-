#ifndef GAME_H
#define GAME_H

class Game
{
private:
    int m_whitePlayerId;
    int m_blackPlayerId;
    int m_result;

public:
    Game(int w, int b, int res = -2);

    int getWhitePlayerId() const;
    int getBlackPlayerId() const;
    int getResult() const;

    void setWhitePlayerId(int);
    void setBlackPlayerId(int);
    void setResult(int);

    bool isResultDefined() const;

};

#endif // GAME_H
