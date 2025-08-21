#include "mainwindow.h"
#include <QDebug>


void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(1);
        deleteTournamentDetailes();
        currentTournament = nullptr;
        ui->pushButtonOKDrawing->setVisible(true);

        ui->lineEditOfTourCount->setDisabled(false);
        ui->PushButtonOkOfNewTournamnet->setDisabled(false);
        ui->pushButtonAddName->setDisabled(false);
    });

   // QObject::connect(ui->pushButtonAddName, &QPushButton::clicked, this, &MainWindow::pushButtonAddName_clicked);

    QObject::connect(ui->pushButtonAddName, &QPushButton::clicked, this, [=](){
        emit pushButtonAddName_clicked("");
    });

    QObject::connect(this, &MainWindow::pushButtonAddName_clicked, this, &MainWindow::on_pushButtonAddName_clicked);

    // Create a container widget inside the scrollAreaOfPlayersName
    QWidget* container1 = new QWidget();
    container1->setLayout(ui->verticalLayoutOfNames);
    ui->scrollAreaOfPlayersName->setWidget(container1);

    // Create a container widget inside the scrollAreaOfTournaments
    QWidget* container2 = new QWidget();
    container2->setLayout(ui->verticalLayoutOfTournamnets);
    ui->scrollAreaOfTournaments->setWidget(container2);

    //edit and delete of Tournaments
    ui->pushButtonEdit->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);


    QObject::connect(ui->pushButtonDelete, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->tableWidgetOfTabel->clear();
    });

    //ok and cancel of new tournamnet
    QObject::connect(ui->cancelPushbutton,  &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
        deleteTournamentDetailes();
    });

    QObject::connect(ui->PushButtonOkOfNewTournamnet, &QPushButton::clicked, this, [this]()
    {
        MainWindow::on_PushButtonOkOfNewTournamnet_clicked(currentTournament);

    });

    //Drowing
    ui->infoTab->setReadOnly(true);
    ui->pushButtonNext->setDisabled(true);
    ui->pushButtonPrevious->setDisabled(true);
    ui->tabWidget->setCurrentIndex(0);
    ui->labelOfTour->setText("Tour 1");
 }

void MainWindow::clearLayout(QLayout* layout)
 {
    if (!layout) return;

    QLayoutItem* item ;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr);
            delete widget;
        } else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);      // Recursively clear nested layouts
            delete childLayout;
        }
    }
}



void MainWindow::deleteTournamentDetailes()
 {
    ui->lineEditOfName ->clear();
    ui->lineEditOfData ->clear();
    ui->lineEditOfTourCount ->clear();
    ui->textEditOfInfo->clear();
    clearLayout(ui->verticalLayoutOfNames);

 }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectFunction();
    currentTournament = nullptr;
};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAddName_clicked(QString text)
{
    QHBoxLayout *horizontalLayoutOfName = new QHBoxLayout();
    QLineEdit *newLine = new QLineEdit();
    newLine->setMinimumHeight(10);
    newLine->setText(text);

    QLabel *newLabel = new QLabel();
    newLabel->setMinimumWidth(15);

    int count = ui->verticalLayoutOfNames->count();
    newLabel->setNum(++count);

    horizontalLayoutOfName->addWidget(newLabel);
    horizontalLayoutOfName->addWidget(newLine);

    ui->verticalLayoutOfNames->addLayout(horizontalLayoutOfName);

}


void MainWindow::addPlayersToGameManager(GameManager* gameManager)
{
    int rowCount = ui->verticalLayoutOfNames->count();
    int lastcount = gameManager->getPlayerCount();
    gameManager->setPlayerCount(ui->verticalLayoutOfNames->count());

    for (int i =0 ; i < rowCount; ++i) {
        QLayoutItem* rowItem = ui->verticalLayoutOfNames->itemAt(i);
        if (!rowItem) continue;

        QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(rowItem->layout());
        if (!rowLayout) continue;

        QLayoutItem* nameItem = rowLayout->itemAt(1);
        if (!nameItem) continue;

        QWidget* widget = nameItem->widget();
        if (!widget) continue;

        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
        if (!lineEdit) continue;

        if(i>=lastcount)
        {
            QString playerName = lineEdit->text().trimmed();
            if (!playerName.isEmpty()) {
                Player* newPlayer = new Player();
                newPlayer->setName(playerName);
                newPlayer->setColorCoef(0);
                newPlayer->setLastColor(0);
                newPlayer->setCurrentPoint(0);
                newPlayer->setId(++lastcount);
                gameManager->addNewPlayer(newPlayer);
            }
        }
        else
        {
            gameManager->getPlayerById(i+1)->setName(lineEdit->text().trimmed());
        }

    }
}

bool MainWindow::isDataComplete( )
{
    try
    {
        if (ui->lineEditOfName->text().isEmpty())
            throw " The 'Name' field is empty. ";


        if (ui->lineEditOfData->text().isEmpty())
            throw " The 'Date' field is empty. ";

        if (ui->lineEditOfTourCount->text().isEmpty())
            throw " The 'Tour Count' field is empty. ";

        if (!(ui->lineEditOfTourCount->text().toInt()))
            throw " The 'Tour Count' should be an integer. ";

        if (ui->textEditOfInfo->document()->isEmpty())
            throw " The 'Info' field is empty. ";

        if(!(ui->verticalLayoutOfNames->count() >= 2))
            throw " The count of the players should be at least two. ";

        int rowCount = ui->verticalLayoutOfNames->count();

        int maxcount =((rowCount*(rowCount-1)/2)/(rowCount/2));

        if( maxcount < ui->lineEditOfTourCount->text().toInt() || !ui->lineEditOfTourCount->text().toInt())
            throw std::string("The count of tour is bigger or smaller than it should be , it can be maximum: ") + std::to_string(maxcount);

            for (int i = 0; i < rowCount; ++i)
        {
                QLayoutItem* rowItem = ui->verticalLayoutOfNames->itemAt(i);
                if (!rowItem) continue;

                QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(rowItem->layout());
                if (!rowLayout) continue;

                QLayoutItem* nameItem = rowLayout->itemAt(1);
                if (!nameItem) continue;

                QWidget* widget = nameItem->widget();
                if (!widget) continue;

                QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
                if (!lineEdit) continue;

                if(lineEdit->text().isEmpty())
                    throw " The data of the players is not complete. ";
            }
            return true;
    }
    catch (const std::string& error)
    {
        QMessageBox::warning(this, "Validation Error", QString::fromStdString(error));
        return false;
    }
    catch (const char* error)
    {
        QMessageBox::warning(this, "Validation Error", QString(error));
        return false;
    }
}

void MainWindow::on_PushButtonOkOfNewTournamnet_clicked(GameManager * thechangingTournamnet)
{
        if(isDataComplete())
        {
            ui->labelOfTour->setText("Tour 1");
            if(thechangingTournamnet)
            {

                thechangingTournamnet->setTourName(ui->lineEditOfName->text());
                thechangingTournamnet->setDate(ui->lineEditOfData->text());
                thechangingTournamnet->setInfo(ui->textEditOfInfo->toPlainText());

                addPlayersToGameManager(thechangingTournamnet);
                vectorOfRadioButtons[thechangingTournamnet->getIndexOfTournament()]->setText(ui->lineEditOfName->text());
                ui->stackedWidget->setCurrentIndex(2);
                 ui->tableWidgetOfDrawing->clear();
                GivingDataToDrawing(thechangingTournamnet);

                // if(thechangingTournamnet->isTheTournamentStarted())
                // {

                // }

            }
            else
            {

                ui->stackedWidget->setCurrentIndex(2);
                QRadioButton * radioButton = new QRadioButton();
                radioButton->setText(ui->lineEditOfName->text());


                ui->verticalLayoutOfTournamnets->addWidget(radioButton);
                vectorOfRadioButtons.push_back(radioButton);

                ui->lineEditOfTourCount->setDisabled(false);
                ui->pushButtonAddName->setDisabled(false);

                static int theIndexOfTurnamnets = 0;

                GameManager* Tournament = new GameManager();
                Tournament->setTourName(ui->lineEditOfName->text());
                Tournament->setTourCount(ui->lineEditOfTourCount->text().toInt());
                Tournament->setDate(ui->lineEditOfData->text());
                Tournament->setInfo(ui->textEditOfInfo->toPlainText());
                Tournament->setIndexOfTournament(theIndexOfTurnamnets++);

                addPlayersToGameManager(Tournament);
                vectorOfTournaments.push_back(Tournament);

                int playerCount = Tournament->getPlayerCount();
                int currentTour = Tournament->getCurrentTour();
                for(int i=1; i<=playerCount; i++)
                {
                    Game * newGame = new Game(i);
                    if(i+1<=playerCount)
                        newGame->setBlackPlayerId(++i);
                    Tournament->setGame(currentTour, newGame);
                }


                QObject::connect(radioButton, &QRadioButton::clicked, this, [Tournament, this](){
                    ui->pushButtonEdit->setDisabled(false);
                    ui->pushButtonDelete->setDisabled(false);
                    ui->stackedWidget->setCurrentIndex(2);
                    currentTournament = Tournament;
                    GivingDataToDrawing(Tournament);
                    deleteTournamentDetailes();
                });

                emit radioButton->click();
            }
            deleteTournamentDetailes();
        }
        else
        {
            return;
        }
}




void MainWindow::GivingDataToDrawing(GameManager* Tournament)
{
    ui->tableWidgetOfDrawing->clear();
    int rowCount = Tournament->getPlayerCount()%2?Tournament->getPlayerCount()/2 +1 : Tournament->getPlayerCount()/2;
    ui->tableWidgetOfDrawing->setRowCount(rowCount);
    ui->tableWidgetOfDrawing->setColumnCount(3);
    ui->infoTab->setText(Tournament->getInfo());

    QStringList headers;
    headers << "White Color" << "Black Color" << "Results";
    ui->tableWidgetOfDrawing->setHorizontalHeaderLabels(headers);

    std::vector<Game*>* games = Tournament->getTourGames(Tournament->getCurrentTour());
    int size = (*games).size();
    for (int i = 0; i < size; ++i)
    {
        ui->tableWidgetOfDrawing->setCellWidget(
            i,0, new QLabel(
                Tournament->getPlayerById(
                                     (*games)[i]->getWhitePlayerId())->getName()));
        if(!(i ==  size-1 && Tournament->getPlayerCount()%2))
        {
            ui->tableWidgetOfDrawing->setCellWidget(
                i,1, new QLabel(
                    Tournament->getPlayerById(
                                     (*games)[i]->getBlackPlayerId())->getName()));
        }
    }

    for(int i = 0; i<rowCount; i++)
    {
        QComboBox * comboBox = new QComboBox();
        comboBox->addItem("-");
        comboBox->addItem("1-0");
        comboBox->addItem("0-1");
        comboBox->addItem("0.5-0.5");

        if(!(Tournament->getCurrentTour() == Tournament->getSizeOfGameMap()) && Tournament->hasTheTournamentStarted())
        {
            std::vector<Game*>* game = Tournament->getTourGames(Tournament->getCurrentTour());

            switch ((*game)[i]->getResult())
            {
                case -1:
                {
                    comboBox->setCurrentIndex(2);
                    break;
                }

                case 1:
                {
                    comboBox->setCurrentIndex(1);
                    break;

                }
                case 0:
                {
                    comboBox->setCurrentIndex(3);
                    break;
                }
                case -2:
                {
                    comboBox->setCurrentIndex(0);
                    break;
                }

                default:
                    qDebug() << "the results of games is wrong ";
                    break;
            }
            comboBox->setDisabled(true);
            ui->tableWidgetOfDrawing->setCellWidget(i, 2, comboBox);

            // qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i,2))->setDisabled(true);
        }
        else
        {
            ui->tableWidgetOfDrawing->setCellWidget(i, 2, comboBox);
        }
    }

    if(Tournament->getPlayerCount()%2) ui->tableWidgetOfDrawing->cellWidget(rowCount-1, 2)->setDisabled(true);
}




void MainWindow::on_pushButtonEdit_clicked()
{
        ui->stackedWidget->setCurrentIndex(1);
        ui->lineEditOfName->setText(currentTournament->getTourName());
        ui->lineEditOfData->setText(currentTournament->getDate());
        ui->textEditOfInfo->setText(currentTournament->getInfo());
        ui->lineEditOfTourCount->setText(QString::number(currentTournament->getTourCount()));

        int playerCount = currentTournament->getPlayerCount();

        for(int i = 1; i<=playerCount; ++i)
        {
            if (currentTournament && currentTournament->getPlayerById(i)) {
                emit pushButtonAddName_clicked(currentTournament->getPlayerById(i)->getName());
            }
        }

        if(currentTournament->hasTheTournamentStarted())
        {
            ui->lineEditOfTourCount->setDisabled(true);
            ui->pushButtonAddName->setDisabled(true);
        }

        ui->pushButtonEdit->setDisabled(true);

}

void MainWindow::on_pushButtonDelete_clicked()
{
    int index = currentTournament->getIndexOfTournament();
    delete vectorOfTournaments[index]; // delete the pointer
    vectorOfTournaments.erase(vectorOfTournaments.begin() + index); // remove from tournaments
    ui->verticalLayoutOfTournamnets->removeItem(ui->verticalLayoutOfTournamnets->takeAt(index));// remove from radio buttons
    ui->pushButtonEdit->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);
    return;
}


std::vector<std::vector<int>> eraseRowCol(const std::vector<std::vector<int>>& matrix, int i, int j) {
    int n = matrix.size();
    std::vector<std::vector<int>> result;

    for (int r = 0; r < n; ++r) {
        if (r == i || r == j) continue;
        std::vector<int> newRow;
        for (int c = 0; c < n; ++c) {
            if (c == i || c == j) continue;
            newRow.push_back(matrix[r][c]);
        }
        result.push_back(newRow);
    }
    return result;
}

// Now returns both sum and pairs
std::pair<int, std::vector<std::pair<int,int>>> findMaxValueWithPairs(const std::vector<std::vector<int>>& matrix, std::vector<int> indices) {
    size_t n = matrix.size();
    if (n == 2) {
        // Only one choice possible
        return { matrix[0][1], { {indices[0], indices[1]} } };
    }

    std::pair<int, std::vector<std::pair<int,int>>> best = {INT_MIN, {}};

    for (int i = 0; i < n-1; ++i) {
        auto erasedMatrix = eraseRowCol(matrix, n-1, i);

        // Remove those indices from list
        std::vector<int> newIndices;
        for (int k = 0; k < (int)indices.size(); ++k) {
            if (k == n-1 || k == i) continue;
            newIndices.push_back(indices[k]);
        }

        auto subResult = findMaxValueWithPairs(erasedMatrix, newIndices);
        int totalSum = matrix[n-1][i] + subResult.first;

        if (totalSum > best.first) {
            best.first = totalSum;
            best.second.clear();
            best.second.push_back({indices[n-1], indices[i]});
            best.second.insert(best.second.end(), subResult.second.begin(), subResult.second.end());
        }
    }

    return best;
}





void MainWindow::on_pushButtonNext_clicked()
{

    currentTournament->setCurrentTour((currentTournament->getCurrentTour())+1);
    int currentTour = currentTournament->getCurrentTour();

    if(currentTournament->getCurrentTour() == currentTournament->getTourCount())
    {
        ui->pushButtonNext->setDisabled(true);
    }

    ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

    if(currentTour-1 == currentTournament->getSizeOfGameMap() || currentTour == currentTournament->getSizeOfGameMap())
    {
        ui->pushButtonOKDrawing->setVisible(true);
<<<<<<< HEAD
=======
        ui->pushButtonNext->setDisabled(true);

        int playerCount = currentTournament->getPlayerCount();
        int weakestPlayerId = -1;
        if(playerCount%2)
        {
            weakestPlayerId = 1;
            for(int i=2; i<=playerCount; i++)
            {
                if(currentTournament->getPlayerById(weakestPlayerId)->getCurrentPoint()>currentTournament->getPlayerById(i)->getCurrentPoint())
                    weakestPlayerId = i;
            }
        }


        std::vector<int> vectorOfIndices ;

        int adjustedPlayerCount = (playerCount % 2 == 0) ? playerCount : playerCount - 1;

        for(int i=0; i<adjustedPlayerCount; ++i)
        {
            vectorOfIndices.push_back(i);
        }

        std::vector<int> participantPlayers;

        for(int i=1; i<=playerCount; i++)
        {
            if(i != weakestPlayerId) participantPlayers.push_back(i);
        }

        std::vector<std::vector<int>> matrix;
        for(int i=1; i<=playerCount; ++i)
        {
            if( i==weakestPlayerId) continue;
            std::vector<int> v;
            for(int j =1; j<=playerCount; ++j)
            {
                if( j==weakestPlayerId) continue;
                ComfortCoef coef;
                int gameCoef = coef.gameCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                int colorCoef = coef.colorCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                int lastColorCoef = coef.lastColorCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                v.push_back(gameCoef+colorCoef+lastColorCoef);
            }
            matrix.push_back(v);
        }

        std::pair<int, std::vector<std::pair<int,int>>> bestVersion = findMaxValueWithPairs(matrix,vectorOfIndices);

        int count = bestVersion.second.size();
        auto result = bestVersion.second;

        for(int i=0; i<count; ++i)
        {
            currentTournament->setGame(currentTour, new Game(participantPlayers[result[i].first], participantPlayers[result[i].second]));

        }

        if(weakestPlayerId != -1)
            currentTournament->setGame(currentTour, new Game(weakestPlayerId));

>>>>>>> 2a8690d8e54843b20b8d8f1cd273babf655d6315
    }

     ui->tableWidgetOfDrawing->clear();

    GivingDataToDrawing(currentTournament);

    ui->pushButtonPrevious->setDisabled(false);

}


void MainWindow::on_pushButtonPrevious_clicked()
{
    currentTournament->setCurrentTour((currentTournament->getCurrentTour())-1);
<<<<<<< HEAD
    if(currentTournament->getCurrentTour()>=2)
    {
        if(currentTournament->getCurrentTour()==2) ui->pushButtonPrevios->setDisabled(true);
        int count = currentTournament->getCurrentTour();
        currentTournament->setCurrentTour(--count);
    }
    ui->pushButtonOKDrawing->setVisible(false);
=======
    ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

    if(currentTournament->getCurrentTour()==1) ui->pushButtonPrevious->setDisabled(true);

    ui->pushButtonOKDrawing->setVisible(false);
    ui->pushButtonNext->setDisabled(false);

    ui->tableWidgetOfDrawing->clear();
     GivingDataToDrawing(currentTournament);
>>>>>>> 2a8690d8e54843b20b8d8f1cd273babf655d6315
}





void MainWindow::on_pushButtonOKDrawing_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(
        this,
        "Warning ",
        "Are you sure that the  results are correct?",
        QMessageBox::Ok | QMessageBox::Cancel
        );

    if (reply == QMessageBox::Ok)
    {

        std::vector<Game*>* game = currentTournament->getTourGames(currentTournament->getCurrentTour());
        int countOfGames = game->size();
        int countOfPlayers = currentTournament->getPlayerCount();

        if((currentTournament->getPlayerCount())%2)
        {
            currentTournament->getPlayerById(countOfPlayers)->setCurrentPoint(1);
            --countOfGames;
        }

        for(int i = 0; i<countOfGames; ++i)
        {

            if (QComboBox* combo = qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i, 2)))
            {
                int index = combo->currentIndex();
                    switch (index)
                {
                    case 0:
                    {
                        QMessageBox::warning(this, "Input Error. ", "Please select a valid option. ");
                        return;
                    }

                    case 1:
                    {
                        (*game)[i]->setResult(1);
                        double whitePlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->getCurrentPoint();
                        currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->setCurrentPoint(++whitePlayerCurrentPoint);
                        break;

                    }
                    case 2:
                    {
                        (*game)[i]->setResult(-1);
                        double blackPlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->getCurrentPoint();
                        currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->setCurrentPoint(++blackPlayerCurrentPoint);
                        break;
                    }

                    case 3:
                    {
                        (*game)[i]->setResult(0);
                        double whitePlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->getCurrentPoint() + 0.5;
                        currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->setCurrentPoint(whitePlayerCurrentPoint);

                        double blackPlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->getCurrentPoint() + 0.5;
                        currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->setCurrentPoint(blackPlayerCurrentPoint);
                        break;
                    }

                    default:
                        qDebug() << "The combo box Id is wrong. ";
                        break;
                }
                    qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i,2))->setDisabled(true);
            }
            else
            {
                qDebug() << "No combo box in this cell. ";
            }
        }
        ui->pushButtonOKDrawing->setVisible(false);
<<<<<<< HEAD
        if(currentTournament->getTourCount()>=2)ui->pushButtonNext->setDisabled(false);
=======
        if(currentTournament->getTourCount()>=2 && currentTournament->getCurrentTour() < currentTournament->getTourCount())
            ui->pushButtonNext->setDisabled(false);
        else
            ui->pushButtonNext->setDisabled(true);
>>>>>>> 2a8690d8e54843b20b8d8f1cd273babf655d6315
    }
    else
    {
        return;
    }
}


