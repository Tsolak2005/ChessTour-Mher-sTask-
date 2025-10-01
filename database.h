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

    void loadingDataBase(std::vector<GameManager*>& vectorOfTournaments,
                         std::vector<QRadioButton *>& vectorOfRadioButtons,
                         std::map<int, std::vector<QRadioButton*>>& mapOfTabelRadiobuttons,
                         QButtonGroup *radioGroup);

    void removeTournamentAndAllDatasInIt(int index);

    void addNewTournamnet(int tourCount, QString tourName, QString data, QString info);

    void addNewGame(int result, int tour, int tournament, int whitePlayerId, int blackPlayerId);
    void updateGamesData(int whitePlayerId, int blackPlayerId, int result, int tour, int tournament);

    void addNewPlayer(int id, QString name, int tournament);
    void updatePlayersData(double currentPoint, int colorCoef, int lastColor, QString name, int tournament, int id);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
