#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>

#include "gamemanager.h"
#include <QRadioButton>
#include <QButtonGroup>

class database
{
public:
    database();
    ~database();
    void clearAllTables();

    void loadingDataBase(std::vector<std::shared_ptr<GameManager>> &vectorOfTournaments,
                                   std::vector<std::shared_ptr<QRadioButton>> &vectorOfRadioButtons,
                                   std::map<int,std::vector<std::shared_ptr<QRadioButton>>> &mapOfTableRadiobuttons,
                         std::shared_ptr<QButtonGroup>& radioGroup);

    void removeTournamentAndAllDatasInIt(int index);

    void addNewTournamnet(int tourCount, QString tourName, QString data, QString info);

    void addNewGame(int result, int tour, int tournament, int whitePlayerId, int blackPlayerId);
    void updateGamesData(int whitePlayerId, int blackPlayerId, int result, int tour, int tournament);

    void addNewPlayer(int id, QString name, int tournament);
    void updatePlayersData(double currentPoint, double extraPoint, int colorCoef, int lastColor, QString name, int tournament, int id);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
