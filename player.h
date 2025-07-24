#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
 
class Player:public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    
    int getCurrentPoint()const;
    int getColorCoef()const;
    int getLastColor()const;
    void setCurrentPoint(const int currentPoint);

private:
    int m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
};

#endif
