#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include<QString>

class Player:public QObject
{
    Q_OBJECT
public:
    Player();
    // Player(Player *parent);

    int getId()const;
    double getCurrentPoint() const;
    double getExtraPoint() const;
    int getColorCoef()const;
    int getLastColor()const;
    QString getName() const;

    void setId(const int playerId);
    void setCurrentPoint(const double currentPoint);
    void setColorCoef(const int colorCoef);
    void setLastColor(const int lastColor);
    void setName(const QString name);
    void addExtraPoint(double point);

private:
    double m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
    int m_id;
    QString m_name;
    double m_extrapoint;
};

#endif
