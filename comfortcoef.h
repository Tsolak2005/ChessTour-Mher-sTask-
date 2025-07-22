#ifndef COMFORTCOEF_H
#define COMFORTCOEF_H

#include "player.h"

class ComfortCoef
{
public:
    ComfortCoef();
    int gameCoef(Player P1, Player P2);
    int colorCoef(Player P1, Player P2);
    int lastColorCoef(Player P1, Player P2);
};

#endif // COMFORTCOEF_H
