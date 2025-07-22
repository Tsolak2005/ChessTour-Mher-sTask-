#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
 
class Player:public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    
private :

};

#endif // PLAYER_H
