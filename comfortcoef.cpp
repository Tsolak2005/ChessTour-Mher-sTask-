#include "comfortcoef.h"
#include "player.h"
#include <QtMath>

ComfortCoef::ComfortCoef() {}

int ComfortCoef::gameCoef(Player P1, Player P2)
{
    return 10 - abs(P1.getCurrentPoint() - P2.getCurrentPoint()) * 2;
}

int ComfortCoef::colorCoef(Player P1, Player P2)
{
    if (abs(P1.getColorCoef() + P2.getColorCoef()) == 0 && P1.getColorCoef()!= 2)
        return 8;
    if (abs(P1.getColorCoef() + P2.getColorCoef()) == 1)
        return 7;
    if (abs(P1.getColorCoef() + P2.getColorCoef()) == 2)
        return 6;
    if (abs(P1.getColorCoef() + P2.getColorCoef()) == 3)
        return 5;
    if (abs(P1.getColorCoef() + P2.getColorCoef()) == 4)
        return 0;
    return 6; //(2,-2) case
}

int ComfortCoef::lastColorCoef(Player P1, Player P2)
{
    return (P1.getLastColor() + 1) % 2 == P2.getLastColor() ? 2 : 1;
}
