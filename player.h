#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include<QString>

struct TieBreakInfo {
    double points = 0.0;    // Total points scored by the player
    double buchholz = 0.0;  // Sum of opponents' points
};

class Player:public QObject
{
    Q_OBJECT
public:
    Player();
    Player(Player *parent);
    int getId()const;
    double getCurrentPoint()const;
    int getColorCoef()const;
    int getLastColor()const;
    QString getName() const;
    void setId(const int playerId);
    void setCurrentPoint(const double currentPoint);
    void setColorCoef(const int colorCoef);
    void setLastColor(const int lastColor);
    void setName(const QString name);
    TieBreakInfo& getTieBreakInfo();

private:
    double m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
    int m_id;
    QString m_name;
    TieBreakInfo m_info;
};

#endif
