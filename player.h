#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include<QString>

class Player:public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    int getId()const;
    int getCurrentPoint()const;
    int getColorCoef()const;
    int getLastColor()const;
    QString getName() const;
    void setId(const int playerId);
    void setCurrentPoint(const int currentPoint);
    void setColorCoef(const int colorCoef);
    void setLastColor(const int lastColor);
    void setName(const QString name);
private:
    int m_currentPoint;
    int m_colorCoef;
    int m_lastColor;
    int m_id;
    QString m_name;
};

#endif
