#include "comfortcoef.h"
#include"player.h"
#include<QtMath>
ComfortCoef::ComfortCoef() {}
int ComfortCoef::GameCoef(Player P1, Player P2) {
    return 10 - abs(P1.GetCurrentPoint() - P2.GetCurrentPoint()) * 2;
}

int ComfortCoef::ColorCoef(Player P1, Player P2){
    if (abs(P1.GetColorCoef() + P2.GetColorCoef()) == 0 && P1.GetColorCoef()!= 2)
        return 8;
    if (abs(P1.GetColorCoef() + P2.GetColorCoef()) == 1)
        return 7;
    if (abs(P1.GetColorCoef() + P2.GetColorCoef()) == 2)
        return 6;
    if (abs(P1.GetColorCoef() + P2.GetColorCoef()) == 3)
        return 5;
    if (abs(P1.GetColorCoef() + P2.GetColorCoef()) == 4)
        return 0;
    return 6; //(2,-2) case
}

int ComfortCoef::LastColorCoef(Player P1, Player P2){
    if (P1.GetLastColor() + P2.GetLastColor() == 1)
        return 2;
    return 1;
}
