
#include "player.h"


Player::Player():m_currentPoint(0), m_colorCoef(0), m_lastColor(-1)
{
}

Player::Player(Player *parent ):m_currentPoint(parent->m_currentPoint),
    m_colorCoef(parent->m_colorCoef), m_lastColor(parent->m_lastColor),
    m_id(parent->m_id), m_name(parent->m_name){}

int Player::getId() const
{
    return m_id;
}

double Player::getCurrentPoint()const
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

void Player::setCurrentPoint(const double currentPoint)
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

void Player::addExtraPoint(double point)
{
    m_extrapoint+=point/10;
}

double Player::getExtraPoint() const
{
    return m_extrapoint;
}



