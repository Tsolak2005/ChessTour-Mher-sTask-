#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
 
class Player:public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    
    int getCurrentPoint();
    int getColorCoef();
    int getLastColor();
    void setCurrentPOint(const int currentPoint);

private:
    int m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
};

#endif
