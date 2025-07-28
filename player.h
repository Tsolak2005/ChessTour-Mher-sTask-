#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>

class Player:public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    int getId()const;
    int getCurrentPoint()const;
    int getColorCoef()const;
    int getLastColor()const;
<<<<<<< HEAD
    void setId(const int id);
=======
    void setId(const int playerId);
>>>>>>> 605ff60d3de0e1eac93279037e79cff283b90150
    void setCurrentPoint(const int currentPoint);
    void setColorCoef(const int colorCoef);
    void setLastColor(const int lastColor);

private:
    int m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
    int m_id;
};

#endif
