#ifndef COMFORTCOEF_H
#define COMFORTCOEF_H
#include"player.h"
class ComfortCoef
{
public:
    ComfortCoef();
    int GameCoef(Player P1, Player P2);
    int ColorCoef(Player P1, Player P2);
    int LastColorCoef(Player P1, Player P2);
};

#endif // COMFORTCOEF_H
