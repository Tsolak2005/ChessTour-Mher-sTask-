#include "tiebreaks.h"

tiebreaks::tiebreaks() {}

std::vector<double> tiebreaks::computeExtraPointsOnly(
    const std::vector<Player*>& players,
    const std::map<int, std::vector<Game*>>& gameMap,
    int upToTour)
{
    std::map<int, Player*> idToPlayer;
    std::map<int, std::vector<int>> opponents;

    // Reset points
    for (auto* p : players) {
        idToPlayer[p->getId()] = p;
        p->getTieBreakInfo().points = 0.0;
    }

    // Step 1: accumulate points and record opponents
    for (auto& [tour, games] : gameMap) {
        if (tour > upToTour) continue;

        for (Game* g : games) {
            int w = g->getWhitePlayerId();
            int b = g->getBlackPlayerId();
            int res = g->getResult();

            Player* wp = idToPlayer[w];
            Player* bp = idToPlayer[b];
            if (!wp || !bp) continue;

            // Update points
            if (res == 1) wp->getTieBreakInfo().points += 1.0;
            else if (res == -1) bp->getTieBreakInfo().points += 1.0;
            else if (res == 0) {
                wp->getTieBreakInfo().points += 0.5;
                bp->getTieBreakInfo().points += 0.5;
            }

            // Record opponents
            opponents[w].push_back(b);
            opponents[b].push_back(w);
        }
    }

    // Step 2: compute Buchholz (extra points only)
    std::vector<double> result;
    for (auto* p : players) {
        double sum = 0.0;
        for (int oppId : opponents[p->getId()]) {
            sum += idToPlayer[oppId]->getTieBreakInfo().points;
        }
        result.push_back(sum);
    }

    return result;
}
