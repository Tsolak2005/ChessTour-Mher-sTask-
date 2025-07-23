#include "player.h"

Player::Player(QObject *parent) {}

int Player::getCurrentPoint()
{
    return m_currentPoint;
}

int Player::getColorCoef()
{
    return m_colorCoef;
}

int Player::getLastColor()
{
    return m_lastColor;
}

void Player::setCurrentPOint(const int currentPoint)
{
    m_currentPoint = currentPoint;
}

bool Player::isPosibleToPlay()
{

}
