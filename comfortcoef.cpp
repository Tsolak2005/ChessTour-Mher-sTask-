#include "comfortcoef.h"
#include"player.h"
#include<QtMath>
ComfortCoef::ComfortCoef() {}
int ComfortCoef::GameCoef(Player P1, Player P2) {
    return 10 - abs(P1.gameCoef - P2.gameCoef ) * 2;
}

int ComfortCoef::ColorCoef(Player P1, Player P2){
    if (abs(P1.colorCoef + P2.colorCoef) == 0 && P1.colorCoef!=2)
        return 8;
    if (abs(P1.colorCoef + P2.colorCoef) == 1)
        return 7;
    if (abs(P1.colorCoef + P2.colorCoef) == 2)
        return 6;
    if (abs(P1.colorCoef + P2.colorCoef) == 3)
        return 5;
    if (abs(P1.colorCoef + P2.colorCoef) == 4)
        return 0;
    return 6; //(2,-2) case
}

int ComfortCoef::LastColorCoef(Player P1, Player P2){
    if (P1.lastColor + P2.lastColor == 1)
        return 2;
    return 1;
}
