#ifndef TIEBREAKS_H
#define TIEBREAKS_H
#include "player.h"
#include "game.h"

class tiebreaks
{
public:
    tiebreaks();
    static std::vector<double> computeExtraPointsOnly(
        const std::vector<Player*>& players,
        const std::map<int, std::vector<Game*>>& gameMap,
        int upToTour);
};

#endif // TIEBREAKS_H
