#ifndef COMFORTCOEF_H
#define COMFORTCOEF_H

#include "player.h"

class ComfortCoef
{
public:
    ComfortCoef();
    int gameCoef(std::shared_ptr<Player>& P1, std::shared_ptr<Player>& P2);
    int colorCoef(std::shared_ptr<Player>& P1, std::shared_ptr<Player>& P2);
    int lastColorCoef(std::shared_ptr<Player> &P1, std::shared_ptr<Player>& P2);
};

#endif // COMFORTCOEF_H
