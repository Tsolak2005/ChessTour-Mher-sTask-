#include "database.h"

database::database()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.sqlite");
    QSqlQuery query(db);


    if (!db.open()) {
        qDebug() << "DB connection failed:" << db.lastError().text();
    } else {
        qDebug() << "DB connected!";
    }

    if(query.exec("CREATE TABLE IF NOT EXISTS tableOfTournaments("
                    "\"index\" INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "tourCount INTEGER,"
                    "tourName TEXT,"
                    "data TEXT,"
                   "info TEXT)"))
    {
        qDebug() << "tableOfTournaments is created successfully";
    }

    if(query.exec("CREATE TABLE IF NOT EXISTS tableOfPlayers("
                   "\"index\" INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "currentPoint REAL,"
                   "colorCoef INTEGER,"
                   "lastColor INTEGER,"
                   "id INTEGER,"
                   "name TEXT,"
                   "tournament INTEGER)"))
    {
        qDebug() << "tabelOfPlayers is created successfully";
    }
    if(query.exec("create table if not exists tableOfGames("
                   "\"index\" INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "whitePlayerId INTEGER,"
                   "blackPLayerId INTEGER,"
                   "result INTEGER,"
                   "tour INTEGER,"
                   "tournament INTEGER)"))
    {
        qDebug() << "tabelOfGames is created successfully";
    }


}

database::~database()
{
    clearAllTables();
}

void database::clearAllTables()
{
    QSqlQuery query(db);

    if (!query.exec("DELETE FROM tableOfPlayers")) {
        qDebug() << "Failed to clear tableOfPlayers:" << query.lastError().text();
    } else {
        qDebug() << "Cleared tableOfPlayers successfully";
    }

    if (!query.exec("DELETE FROM tableOfGames")) {
        qDebug() << "Failed to clear tableOfGames:" << query.lastError().text();
    } else {
        qDebug() << "Cleared tableOfGames successfully";
    }

    if (!query.exec("DELETE FROM tableOfTournaments")) {
        qDebug() << "Failed to clear tableOfTournaments:" << query.lastError().text();
    } else {
        qDebug() << "Cleared tableOfTournaments successfully";
    }

    if (!query.exec("DELETE FROM sqlite_sequence WHERE name='tableOfPlayers'")) {
        qDebug() << "Failed to reset AUTOINCREMENT for tableOfPlayers:" << query.lastError().text();
    }
    if (!query.exec("DELETE FROM sqlite_sequence WHERE name='tableOfGames'")) {
        qDebug() << "Failed to reset AUTOINCREMENT for tableOfGames:" << query.lastError().text();
    }
    if (!query.exec("DELETE FROM sqlite_sequence WHERE name='tableOfTournaments'")) {
        qDebug() << "Failed to reset AUTOINCREMENT for tableOfTournaments:" << query.lastError().text();
    }

    qDebug() << "All tables cleared and AUTOINCREMENT reset.";
}

void database::addNewTournamnet(int tourCount, QString tourName, QString data, QString info)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO tableOfTournaments (tourCount, tourName, data, info) "
                  "VALUES (:tourCount, :tourName, :data, :info)");

    query.bindValue(":tourCount", tourCount);
    query.bindValue(":tourName", tourName);
    query.bindValue(":data", tourName);
    query.bindValue(":info", info);

    if(query.exec())
    {
        qDebug() << "tournament datas inserted successfully";
    }
    else
    {
        qDebug() << " ERORR: tournament datas are not inserted";
        qDebug() << "SQL error:" << query.lastError().text();
    }

}

void database::removeTournamentAndAllDatasInIt(int index)
{
    QSqlQuery query(db);

    query.prepare("DELETE FROM tableOfTournaments WHERE \"index\" = :index ");

    query.bindValue(":index" , index);

    if (query.exec()) {
        qDebug() << "Tournamnet with index: " <<index << " is deleted successfully";
    } else {
        qDebug() << "EROR!! Tournamnet with index : "<< index << " is not deleted";
    }

    // ry.prep
}

void database::addNewPlayer(int id, QString name, int tournament)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO tableOfPlayers (currentPoint, colorCoef, lastColor, id, name, tournament) "
                  "VALUES (:currentPoint, :colorCoef, :lastColor, :id, :name, :tournament)");

    query.bindValue(":currentPoint", 0);
    query.bindValue(":colorCoef", 0);
    query.bindValue(":lastColor", 0);
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":tournament", tournament);

    if (query.exec()) {
        int newIndex = query.lastInsertId().toInt();
        qDebug() << "Player inserted with index:" << newIndex
                 << "and logical id:" << id;
    } else {
        qDebug() << "ERROR inserting player:" << query.lastError().text();
    }
}

void database::addNewGame(int result, int tour, int tournament, int whitePlayerId, int blackPlayerId)
{
    QSqlQuery query(db);

    query.prepare("INSERT INTO tableOfGames (whitePlayerId, blackPlayerId, result, tour, tournament)"
                   "VALUES (:whitePlayerId, :blackPlayerId, :result, :tour, :tournament)");

    query.bindValue(":whitePlayerId", whitePlayerId);
    query.bindValue(":blackPlayerId", blackPlayerId);
    query.bindValue(":result", result);
    query.bindValue(":tour", tour);
    query.bindValue(":tournament", tournament);

    if(query.exec())
    {
        qDebug() << "Game datas inserted successfully";
    }
    else
    {
        qDebug() << " ERORR: Game datas are not inserted";
        qDebug() << "SQL error:" << query.lastError().text();
    }

}

void database::updateGamesData(int whitePlayerId, int blackPlayerId, int result, int tour, int tournament)
{
    QSqlQuery query(db);

    query.prepare("UPDATE tableOfGames "
                  "SET result = :result "
                  "WHERE tour = :tour AND "
                  "tournament = :tournament AND "
                  "whitePlayerId = :whitePlayerId AND "
                  "blackPlayerId = :blackPlayerId");

    query.bindValue(":whitePlayerId", whitePlayerId);
    query.bindValue(":blackPlayerId", blackPlayerId);
    query.bindValue(":result", result);
    query.bindValue(":tour", tour);
    query.bindValue(":tournament", tournament);

    if (query.exec()) {
        qDebug() << "Game updated successfully!";
    } else {
        qDebug() << "Failed to update game:" << query.lastError().text();
    }
}

void database::updatePlayersData(double currentPoint, int colorCoef, int lastColor, QString name, int tournament, int id)
{
    QSqlQuery query(db);

    query.prepare("UPDATE tableOfPlayers SET name = :name, currentPoint = :currentPoint, colorCoef = :colorCoef,"
                  " lastColor = :lastColor where id = :id and tournament = :tournament");

    query.bindValue(":currentPoint", currentPoint);
    query.bindValue(":colorCoef", colorCoef);
    query.bindValue(":lastColor", lastColor);
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":tournament", tournament);


    if(query.exec())
    {
        qDebug() << "player's datas with id:" << QString::number(id) <<"changed successfully";
    }
    else
    {
        qDebug() << " ERORR: player's datas are not inserted";
        qDebug() << "SQL error:" << query.lastError().text();
    }
}
