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
    // clearAllTables();
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

void database::loadingDataBase(std::vector<GameManager *> &vectorOfTournaments,
                               std::vector<QRadioButton *> &vectorOfRadioButtons,
                               std::map<int, std::vector<QRadioButton *>> &mapOfTabelRadiobuttons,
                               QButtonGroup *radioGroup)
{
    QSqlQuery queryOfTournamnets(db);
    QSqlQuery queryOfPlayers(db);
    QSqlQuery queryOfGames(db);

    if (!queryOfTournamnets.exec("SELECT * FROM tableOfTournaments")) {
        qDebug() << "Failed to load tournaments:" << queryOfTournamnets.lastError().text();
        return;
    }

    while (queryOfTournamnets.next()) {
        int tournamentId   = queryOfTournamnets.value("index").toInt();
        int tourCount      = queryOfTournamnets.value("tourCount").toInt();
        QString tourName   = queryOfTournamnets.value("tourName").toString();
        QString data       = queryOfTournamnets.value("data").toString();
        QString info       = queryOfTournamnets.value("info").toString();

        queryOfPlayers.prepare("SELECT COUNT(*) FROM tableOfPlayers where tournament = :tournament");
        queryOfPlayers.bindValue(":tournament", tournamentId);

        int playerCount = 0;
        if (queryOfPlayers.exec() && queryOfPlayers.next()) {

            playerCount = queryOfPlayers.value(0).toInt();
        }

        // 1. Create GameManager for this tournament
        GameManager *Tournament = new GameManager(playerCount);
        Tournament->setPlayerCount(playerCount);
        Tournament->setIndexOfTournament(tournamentId-1);
        Tournament->setTourCount(tourCount);
        Tournament->setTourName(tourName);
        Tournament->setDate(data);
        Tournament->setInfo(info);

        vectorOfTournaments.push_back(Tournament);


        // 2. Load players for this tournament
        queryOfPlayers.prepare("SELECT * FROM tableOfPlayers where tournament = :tournament");
        queryOfPlayers.bindValue(":tournament", tournamentId);

        if (queryOfPlayers.exec())
        {
            while(queryOfPlayers.next())
            {
                    Player *p = new Player();
                    p->setId(queryOfPlayers.value("id").toInt());
                    p->setName(queryOfPlayers.value("name").toString());
                    p->setCurrentPoint(queryOfPlayers.value("currentPoint").toDouble());
                    p->setColorCoef(queryOfPlayers.value("colorCoef").toInt());
                    p->setLastColor(queryOfPlayers.value("lastColor").toInt());
                    Tournament->addNewPlayer(p);
            }
        }


        // 3. Load games for this tournament and creating matrix for players
        int theLastOrganizedTour = 0;

        QSqlQuery gameQuery(db);
        gameQuery.prepare("SELECT * FROM tableOfGames WHERE tournament = :tournament");
        gameQuery.bindValue(":tournament", tournamentId);
        if (gameQuery.exec()) {
            while (gameQuery.next()) {
                int whitePlayerId = gameQuery.value("whitePlayerId").toInt();
                int blackPlayerId = gameQuery.value("blackPlayerId").toInt();
                int res = gameQuery.value("result").toInt();
                int tour = gameQuery.value("tour").toInt();
                if(theLastOrganizedTour<tour)
                {
                    theLastOrganizedTour=tour;
                    if(res == -2)
                    {
                        --theLastOrganizedTour;
                    }
                }
                Game * newGame = new Game(whitePlayerId,blackPlayerId,res);
                Tournament->setGame(tour, newGame);
                if(blackPlayerId > 0)
                Tournament->ThePlayerSMet(whitePlayerId,blackPlayerId);
            }
        }

        //#currentoOganizedTour
        Tournament->setCurrentOganizedTour(++theLastOrganizedTour);

        // 4. Create radio button for UI
        QRadioButton *btn = new QRadioButton(tourName);
        radioGroup->addButton(btn);
        vectorOfRadioButtons.push_back(btn);

        //5 Create radio buttons for Table
        for(int i=0; i<theLastOrganizedTour-1; ++i)
        {
            QRadioButton * radioButtonOfTours = new QRadioButton();
            radioButtonOfTours->setText("Tour " + QString::number(i+1));
            mapOfTabelRadiobuttons[Tournament->getIndexOfTournament()].push_back(radioButtonOfTours);
        }

    }
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
    ++index;

    //REMOVING

    query.prepare("DELETE FROM tableOfTournaments WHERE \"index\" = :index ");

    query.bindValue(":index" , index);

    if (query.exec()) {
        qDebug() << "Tournamnet with index: " <<index << " is deleted successfully";
    } else {
        qDebug() << "EROR!! Tournamnet with index : "<< index << " is not deleted";
    }

    query.prepare("DELETE FROM tableOfPlayers Where tournament = :tournament");

    query.bindValue(":tournament" , index);

    if (query.exec()) {
        qDebug() << "Players' datas in tournament with index: " <<index << " are deleted successfully";
    } else {
        qDebug() << "EROR!! Players' datas in tournament with index : "<< index << " are not deleted";
    }

    query.prepare("DELETE FROM tableOfGames where tournament = :tournament");

    query.bindValue(":tournament" , index);

    if (query.exec()) {
        qDebug() << "Games' datas in tournament with index: " <<index << " are deleted successfully";
    } else {
        qDebug() << "EROR!! Games' datas in tournament with index : "<< index << " are not deleted";
    }

    //UPDATING
    query.prepare("UPDATE tableOfTournaments Set \"index\" = \"index\"-1 where \"index\" > :tournamentId");

    query.bindValue(":tournamentId",index);

    if (query.exec()) {
        qDebug() << "Tournaments insdexes are updated";
    } else {
        qDebug() << "EROR!! Tournaments insdexes are not updated";
    }


    query.prepare("UPDATE tableOfPlayers Set tournament = tournament-1 where tournament > :tournamentId");

    query.bindValue(":tournamentId",index);

    if (query.exec()) {
        qDebug() << "Players' datas are updated";
    } else {
        qDebug() << "EROR!! Players' datas are not updated";
    }


    query.prepare("UPDATE tableOfGames Set tournament = tournament-1 where tournament > :tournamentId");

    query.bindValue(":tournamentId",index);

    if (query.exec()) {
        qDebug() << "Games' datas are updated";
    } else {
        qDebug() << "EROR!! Games' datas are not updated";
    }

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
    query.bindValue(":tournament", tournament+1);

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
    query.bindValue(":tournament", tournament+1);

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
    query.bindValue(":tournament", tournament+1);

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
    query.bindValue(":tournament", tournament+1);


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
