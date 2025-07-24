#include "player.h"

Player::Player(QObject *parent) {}

int Player::getCurrentPoint()const
{
    return m_currentPoint;
}

int Player::getColorCoef()const
{
    return m_colorCoef;
}

int Player::getLastColor()const
{
    return m_lastColor;
}

void Player::setCurrentPoint(const int currentPoint)
{
    m_currentPoint = currentPoint;
}
