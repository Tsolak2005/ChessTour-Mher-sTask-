
#include "player.h"

Player::Player(QObject *parent) {}

int Player::getId() const
{
    return m_id;
}

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

QString Player::getName() const
{
    return m_name;
}

void Player::setId(const int playerId)
{
    m_id = playerId;
}

void Player::setCurrentPoint(const int currentPoint)
{
    m_currentPoint = currentPoint;
}

void Player::setColorCoef(const int colorCoef)
{
    m_colorCoef = colorCoef;
}

void Player::setLastColor(const int lastColor)
{
    m_lastColor = lastColor;
}

void Player::setName(const QString name)
{
    m_name = name;
}
