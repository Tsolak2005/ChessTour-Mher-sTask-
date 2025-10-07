#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/* void MergeSortForPlayers(std::vector<Player*>& Players, int start, int end,
                         std::vector<double>& Scores,
                         std::vector<double>& ExtraPoints)   // pass by reference
{
    if (end - start < 2) return;

    if (end - start == 2) {
        // compare by score first, then by extraPoints
        if (Scores[start] < Scores[start+1] ||
            (Scores[start] == Scores[start+1] && ExtraPoints[start] < ExtraPoints[start+1]))
        {
            std::swap(Players[start], Players[start+1]);
            std::swap(Scores[start], Scores[start+1]);
            std::swap(ExtraPoints[start], ExtraPoints[start+1]);
        }
        return;
    }

    int mid = start + (end - start) / 2;

    MergeSortForPlayers(Players, start, mid, Scores, ExtraPoints);
    MergeSortForPlayers(Players, mid, end, Scores, ExtraPoints);

    // merge step
    std::vector<Player*> tempPlayers;
    std::vector<double> tempScores;
    std::vector<double> tempExtra;

    tempPlayers.reserve(end - start);
    tempScores.reserve(end - start);
    tempExtra.reserve(end - start);

    int i = start;
    int j = mid;

    while (i < mid && j < end)
    {
        if (Scores[i] > Scores[j] ||
            (Scores[i] == Scores[j] && ExtraPoints[i] >= ExtraPoints[j]))
        {
            tempPlayers.push_back(Players[i]);
            tempScores.push_back(Scores[i]);
            tempExtra.push_back(ExtraPoints[i]);
            i++;
        }
        else {
            tempPlayers.push_back(Players[j]);
            tempScores.push_back(Scores[j]);
            tempExtra.push_back(ExtraPoints[j]);
            j++;
        }
    }

    while (i < mid) {
        tempPlayers.push_back(Players[i]);
        tempScores.push_back(Scores[i]);
        tempExtra.push_back(ExtraPoints[i]);
        i++;
    }
    while (j < end) {
        tempPlayers.push_back(Players[j]);
        tempScores.push_back(Scores[j]);
        tempExtra.push_back(ExtraPoints[j]);
        j++;
    }

    for (int k = 0; k < (int)tempPlayers.size(); k++) {
        Players[start + k] = tempPlayers[k];
        Scores[start + k] = tempScores[k];
        ExtraPoints[start + k] = tempExtra[k];
    }
} */

void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(1);
        deleteTournamentDetailes();
        currentTournament = nullptr;
        ui->pushButtonOkOfDrawing->setVisible(true);

        ui->lineEditOfTourCount->setDisabled(false);
        ui->PushButtonOkOfNewTournamnet->setDisabled(false);
        ui->pushButtonAddName->setDisabled(false);
    });

   // QObject::connect(ui->pushButtonAddName, &QPushButton::clicked, this, &MainWindow::pushButtonAddName_clicked);

    QObject::connect(ui->pushButtonAddName, &QPushButton::clicked, this, [=](){
        on_pushButtonAddName_clicked("");
    });

    // QObject::connect(this, &MainWindow::pushButtonAddName_clicked, this, &MainWindow::on_pushButtonAddName_clicked);

    // Create a container widget inside the scrollAreaOfPlayersName
    QWidget* container1 = new QWidget(this);
    container1->setLayout(ui->verticalLayoutOfNames);
    ui->scrollAreaOfPlayersName->setWidget(container1);

    // Create a container widget inside the scrollAreaOfTournaments
    QWidget* container2 = new QWidget(this);
    container2->setLayout(ui->verticalLayoutOfTournamnets);
    ui->scrollAreaOfTournaments->setWidget(container2);

    // Create a container widget inside the scrollAreaOfOld Tournaments
    QWidget* container3 = new QWidget(this);
    container3->setLayout(ui->verticalLayoutOfOldTournaments);
    ui->scrollAreaOfOldTournaments->setWidget(container3);

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

    //Group Radiobuttns


    //Drowing
    ui->infoTab->setReadOnly(true);
    ui->pushButtonNext->setDisabled(true);
    ui->pushButtonPrevious->setDisabled(true);
    ui->tabWidget->setCurrentIndex(0);
    ui->labelOfTour->setText("Tour 1");

    //Drowing

    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, [this](int index)
        {
            switch (index) {
            case 0:
                break;
            case 1:
                ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
                emit mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->clicked();
                mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->setChecked(true);
                break;
            default:
                break;
            }
        });


    // Sort
    connect(ui->checkBoxOfSort, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state)
    {

        int columnsWithWidgets = 0;
        int columnCount = currentTournament->getTourCount();

        for (int col = 0; col < columnCount; ++col) {
            QWidget* w = ui->tableWidgetOfTabel->cellWidget(0, col); // check only row 0
            if (w) {
                ++columnsWithWidgets;
            }
        }

        if(state == Qt::Checked)
        {
            std::vector<std::shared_ptr<Player>> newPlayerList;
            std::vector<std::shared_ptr<Player>> playerList = currentTournament->getPlayers();
            // if(playerList)
            int playerCount = currentTournament->getPlayerCount();

            for(int i=0; i<playerCount; ++i)
            {
                newPlayerList.push_back(playerList[i]);
            }

            std::vector<double> Scores;

            int tourCount = currentTournament->getTourCount();
            for(int i=0; i<playerCount; ++i)
            {
                double score = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget(i,tourCount))->text().toDouble();
                Scores.push_back(score);
            }

            std::sort(newPlayerList.begin(), newPlayerList.end(), [](const std::shared_ptr<Player>& lhs, const std::shared_ptr<Player>& rhs) {
                return lhs->getCurrentPoint() > rhs->getCurrentPoint() || lhs->getCurrentPoint() == rhs->getCurrentPoint() && lhs->getExtraPoint() > rhs->getExtraPoint();
            });

            currentTournament->changePlayersList(newPlayerList);

            GivingDataToTable(*currentTournament, columnsWithWidgets);

            currentTournament->changePlayersList(playerList);

            // delete newPlayerList;

        }
        else
        {
            GivingDataToTable(*currentTournament, columnsWithWidgets);
        }
    } );
}



void MainWindow::connectionsOfLoadingRadiobuttons()
{
    clearLayout(ui->horizontalLayoutOFTorursOfTabel);

    for (int i = 0; i < vectorOfRadioButtons.size() && i < vectorOfTournaments.size(); ++i)
    {
        std::shared_ptr<QRadioButton>& radioButton = vectorOfRadioButtons[i];
        // GameManager* Tournament = vectorOfTournaments[i].get();

        radioButtonsConnections(radioButton, vectorOfTournaments[i].get());

        if(vectorOfTournaments[i]->getCurrentoOganizedTour()-1==vectorOfTournaments[i]->getTourCount())
        {
            ui->verticalLayoutOfOldTournaments->addWidget(radioButton.get());
        }
        else
        {
            ui->verticalLayoutOfTournamnets->addWidget(radioButton.get());
        }

        int j =1;
        for(auto& radioButtonOfTours: mapOfTableRadiobuttons[i])
        {
            QObject::connect(radioButtonOfTours.get(), &QRadioButton::clicked, this, [this, j]()
                             {
                                 GivingDataToTable(*currentTournament, j);
                                 ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
                             });
            ++j;
        }
    }

}

void MainWindow::radioButtonsConnections(std::shared_ptr<QRadioButton> &radioButton, GameManager* tournament)
{
    QObject::connect(radioButton.get(), &QRadioButton::clicked, this, [tournament, this]()
    {
        ui->stackedWidget->setCurrentIndex(2);
        currentTournament = tournament;
        if(currentTournament->hasTheTournamentStarted())
            currentTournament->setCurrentTour(currentTournament->getCurrentoOganizedTour()-1);


        if(currentTournament->getCurrentoOganizedTour()-1==currentTournament->getTourCount())
        {
            ui->pushButtonEdit->setDisabled(true);
            ui->pushButtonDelete->setDisabled(false);
        }
        else
        {
            ui->pushButtonEdit->setDisabled(false);
            ui->pushButtonDelete->setDisabled(false);
        }

        ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

        if(!currentTournament->hasTheTournamentStarted())
        {
            ui->pushButtonAddName->setDisabled(false);
            ui->lineEditOfTourCount->setDisabled(false);
        }

        if((currentTournament->getCurrentTour() != currentTournament->getTourCount()) &&
            (currentTournament->getCurrentoOganizedTour()!=currentTournament->getCurrentTour()))
        {ui->pushButtonNext->setDisabled(false);}
        else
        {ui->pushButtonNext->setDisabled(true);}

        if(currentTournament->getCurrentTour()==1)
        {
            ui->pushButtonPrevious->setDisabled(true);
        }
        else
        {
            ui->pushButtonPrevious->setDisabled(false);
        }

        if(currentTournament->hasTheTournamentStarted())
        {
            ui->tabWidget->setTabEnabled(1,true);
        }
        else
        {
            ui->tabWidget->setTabEnabled(1,false);
        }

        clearLayout(ui->horizontalLayoutOFTorursOfTabel);


        QLabel * l = new QLabel();
        l->setText("                  ");
        ui->horizontalLayoutOFTorursOfTabel->addWidget(l);

        std::vector<std::shared_ptr<QRadioButton>>& radios = mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()];

        for (auto& rb : radios) {
            ui->horizontalLayoutOFTorursOfTabel->addWidget(rb.get());
        }


        GivingDataToTable(*currentTournament, currentTournament->getCurrentoOganizedTour()-1);

        GivingDataToDrawing(*tournament);

        ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
        if(currentTournament->hasTheTournamentStarted())
        {
            emit mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->clicked();
            mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->setChecked(true);
        }

        deleteTournamentDetailes();
    });
}


void MainWindow::clearLayout(QLayout* layout)
{
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr);
        }
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void MainWindow::removeWidgetFromLayout(QLayout* layout, QWidget* widget)
{
    if (!layout || !widget)
    {
        return;
    }


    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (item && item->widget() == widget) {
            layout->takeAt(i);          // remove item from layout
            widget->setParent(nullptr); // detach widget from layout
            delete item;                // delete only the QLayoutItem (not the widget!)
            break;
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
    , radioGroup(new QButtonGroup(this))
{
    ui->setupUi(this);


    //add ui loading


    connectFunction();
    currentTournament = nullptr;
    radioGroup->setExclusive(true);

    //data loading
    dataBase.loadingDataBase(vectorOfTournaments,vectorOfRadioButtons,mapOfTableRadiobuttons, radioGroup);
    connectionsOfLoadingRadiobuttons();

};

MainWindow::~MainWindow()
{
    // // Clean tournaments
    // for (auto* t : vectorOfTournaments) {
    //     delete t; // free GameManager objects
    // }
    // vectorOfTournaments.clear();

    // // Clean radio buttons
    // for (auto* rb : vectorOfRadioButtons) {
    //     delete rb; // free QRadioButton objects
    // }
    // vectorOfRadioButtons.clear();

    // delete radioGroup;

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


void MainWindow::addPlayersToGameManager(GameManager& gameManager)
{
    int rowCount = ui->verticalLayoutOfNames->count();
    int lastcount = gameManager.getPlayerCount();
    gameManager.setPlayerCount(ui->verticalLayoutOfNames->count());

    for (int i =0 ; i < rowCount; ++i) {
        QLayoutItem* rowItem = ui->verticalLayoutOfNames->itemAt(i);

        QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(rowItem->layout());

        QLayoutItem* nameItem = rowLayout->itemAt(1);

        QWidget* widget = nameItem->widget();

        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);

        if(i >= lastcount)
        {
            QString playerName = lineEdit->text().trimmed();
            if (!playerName.isEmpty())
            {
                std::shared_ptr<Player> newPlayer = std::make_shared<Player>();
                newPlayer->setName(playerName);
                newPlayer->setColorCoef(0);
                newPlayer->setLastColor(-1);
                newPlayer->setCurrentPoint(0);
                newPlayer->setId(++lastcount);
                gameManager.addNewPlayer(newPlayer);

                dataBase.addNewPlayer(lastcount,playerName, currentTournament->getIndexOfTournament());
            }
        }
        else
        {

            QString playerName = lineEdit->text().trimmed();

            gameManager.getPlayerById(i + 1)->setName(playerName);

            dataBase.updatePlayersData(0, 0, 0, -1, playerName,currentTournament->getIndexOfTournament(),i+1);
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

        int maxcount = rowCount - 1;

        if( maxcount < ui->lineEditOfTourCount->text().toInt() || !ui->lineEditOfTourCount->text().toInt())
            throw std::string("The count of tour is bigger or smaller than it should be , it can be maximum: ") + std::to_string(maxcount);

            for (int i = 0; i < rowCount; ++i)
            {
                QLayoutItem* rowItem = ui->verticalLayoutOfNames->itemAt(i);

                QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(rowItem->layout());

                QLayoutItem* nameItem = rowLayout->itemAt(1);

                QWidget* widget = nameItem->widget();

                QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);

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

        if(thechangingTournamnet)
        {

            ui->pushButtonEdit->setDisabled(false);
            thechangingTournamnet->setTourName(ui->lineEditOfName->text());
            thechangingTournamnet->setDate(ui->lineEditOfData->text());
            thechangingTournamnet->setInfo(ui->textEditOfInfo->toPlainText());
            int lastPlayerCount = thechangingTournamnet->getPlayerCount();

            addPlayersToGameManager(*thechangingTournamnet);
            vectorOfRadioButtons[thechangingTournamnet->getIndexOfTournament()]->setText(ui->lineEditOfName->text());
            ui->stackedWidget->setCurrentIndex(2);
            ui->tableWidgetOfDrawing->clear();


            if(thechangingTournamnet->hasTheTournamentStarted())
            {
                ui->labelOfTour->setText("Tour " + QString::number(thechangingTournamnet->getCurrentoOganizedTour()-1));
                ui->tabWidget->setTabEnabled(1,true);
                GivingDataToDrawing(*thechangingTournamnet);
                GivingDataToTable(*thechangingTournamnet, thechangingTournamnet->getCurrentoOganizedTour()-1);
            }
            else
            {
                ui->labelOfTour->setText("Tour 1");
                ui->tabWidget->setTabEnabled(1,false);

                int playerCount = thechangingTournamnet->getPlayerCount();
                int currentTour = thechangingTournamnet->getCurrentTour();
                thechangingTournamnet->setTourCount(ui->lineEditOfTourCount->text().toInt());

                if(lastPlayerCount < playerCount)
                {
                    std::cout << "ERROR HERE" << std::endl;

                    thechangingTournamnet->changeMatrixOfPlayers(playerCount, lastPlayerCount);
                    if(lastPlayerCount % 2){

                        thechangingTournamnet->getTourGames(currentTour).back()->setBlackPlayerId(++lastPlayerCount);
                        thechangingTournamnet->ThePlayerSMet(lastPlayerCount-1, lastPlayerCount);
                    }
                    for(int i=lastPlayerCount+1; i<=playerCount; i++)
                    {
                        std::shared_ptr<Game> newGame = std::make_shared<Game>(i,-1);
                        if(i+1<=playerCount)
                        {
                            thechangingTournamnet->ThePlayerSMet(i,i+1);
                            newGame->setBlackPlayerId(++i);

                            dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),i,i+1);
                        }
                        else
                        {
                            dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),i,-1);
                        }
                        thechangingTournamnet->setGame(currentTour, newGame);
                    }
                }
                GivingDataToDrawing(*thechangingTournamnet);
            }
            deleteTournamentDetailes();


        }
        else
        {
            ui->labelOfTour->setText("Tour 1");
            ui->tableWidgetOfTabel->clear();
            ui->stackedWidget->setCurrentIndex(2);



            std::shared_ptr<QRadioButton> radioButton =  std::make_shared<QRadioButton>();
            radioButton->setText(ui->lineEditOfName->text());



            ui->verticalLayoutOfTournamnets->addWidget(radioButton.get());
            radioGroup->addButton(radioButton.get());

            vectorOfRadioButtons.emplace_back(radioButton);



            ui->lineEditOfTourCount->setDisabled(false);
            ui->pushButtonAddName->setDisabled(false);


            std::shared_ptr<GameManager> Tournament = std::make_shared<GameManager>(ui->verticalLayoutOfNames->count());

            Tournament->setTourName(ui->lineEditOfName->text().trimmed());
            Tournament->setTourCount(ui->lineEditOfTourCount->text().toInt());
            Tournament->setDate(ui->lineEditOfData->text().trimmed());
            Tournament->setInfo(ui->textEditOfInfo->toPlainText().trimmed());

            vectorOfTournaments.emplace_back(Tournament);

            qDebug() << Tournament->getTourCount();

            dataBase.addNewTournamnet(Tournament->getTourCount(),Tournament->getTourName(),Tournament->getDate(), Tournament->getInfo());


            currentTournament = Tournament.get();



            int i = 0;
            for (auto& it : vectorOfTournaments) {
                it->setIndexOfTournament(i++);
            }

            addPlayersToGameManager(*Tournament);



            int playerCount = Tournament->getPlayerCount();
            int currentTour = Tournament->getCurrentTour();

            for(int i=1; i<=playerCount; i++)
            {
                std::shared_ptr<Game> newGame = std::make_shared<Game>(i,-1);

                if(i+1<=playerCount)
                {
                    Tournament->ThePlayerSMet(i,i+1);
                    dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),i,i+1);
                    newGame->setBlackPlayerId(++i);
                }
                else
                {
                    dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),i,-1);
                }
                Tournament->setGame(currentTour, newGame);
            }

            radioButtonsConnections(radioButton,Tournament.get());
            // QObject::connect(radioButton.get(), &QRadioButton::clicked, this, [Tournament, this]()
            //                  {

            //                      ui->stackedWidget->setCurrentIndex(2);
            //                         currentTournament = Tournament.get();
            //                      if(currentTournament->hasTheTournamentStarted())
            //                          currentTournament->setCurrentTour(currentTournament->getCurrentoOganizedTour()-1);

            //                      if(currentTournament->getCurrentoOganizedTour()-1==currentTournament->getTourCount())
            //                      {
            //                          ui->pushButtonEdit->setDisabled(true);
            //                          ui->pushButtonDelete->setDisabled(false);
            //                      }
            //                      else
            //                      {
            //                          ui->pushButtonEdit->setDisabled(false);
            //                          ui->pushButtonDelete->setDisabled(false);
            //                      }

            //                      ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

            //                      if(!currentTournament->hasTheTournamentStarted())
            //                      {
            //                          ui->pushButtonAddName->setDisabled(false);
            //                          ui->lineEditOfTourCount->setDisabled(false);
            //                      }

            //                      if((currentTournament->getCurrentTour() != currentTournament->getTourCount()) &&
            //                          (currentTournament->getCurrentoOganizedTour()!=currentTournament->getCurrentTour()))
            //                      {ui->pushButtonNext->setDisabled(false);}
            //                      else
            //                      {ui->pushButtonNext->setDisabled(true);}

            //                      if(currentTournament->getCurrentTour()==1)
            //                      {
            //                          ui->pushButtonPrevious->setDisabled(true);
            //                      }
            //                      else
            //                      {
            //                          ui->pushButtonPrevious->setDisabled(false);
            //                      }

            //                      if(currentTournament->hasTheTournamentStarted())
            //                      {
            //                          ui->tabWidget->setTabEnabled(1,true);
            //                      }
            //                      else
            //                      {
            //                          ui->tabWidget->setTabEnabled(1,false);
            //                      }

            //                      clearLayout(ui->horizontalLayoutOFTorursOfTabel);


            //                      QLabel * l = new QLabel();
            //                      l->setText("                  ");
            //                      ui->horizontalLayoutOFTorursOfTabel->addWidget(l);


            //                      std::vector<std::shared_ptr<QRadioButton>>& radio = mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()];

            //                      for (auto& rb : radio) {
            //                          ui->horizontalLayoutOFTorursOfTabel->addWidget(rb.get());
            //                      }

            //                      GivingDataToTable(*currentTournament, currentTournament->getCurrentoOganizedTour()-1);
            //                      GivingDataToDrawing(*currentTournament);
            //                      ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
            //                      if(currentTournament->hasTheTournamentStarted())
            //                      {
            //                          emit mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->clicked();
            //                          mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].back()->setChecked(true);
            //                      }


            //                      deleteTournamentDetailes();
            //                  });

            emit radioButton->click();



        }
    }
    else
    {
        return;
    }
}


void MainWindow::GivingDataToDrawing(GameManager& Tournament)
{

    if(Tournament.getCurrentoOganizedTour() == Tournament.getCurrentTour()) ui->pushButtonOkOfDrawing->setVisible(true);


    ui->tableWidgetOfDrawing->clear();
    int rowCount = Tournament.getPlayerCount()%2?Tournament.getPlayerCount()/2 +1 : Tournament.getPlayerCount()/2;
    ui->tableWidgetOfDrawing->setRowCount(rowCount);
    ui->tableWidgetOfDrawing->setColumnCount(3);
    ui->infoTab->setText(Tournament.getInfo());



    QStringList headers;
    headers << "White Color" << "Black Color" << "Results";
    ui->tableWidgetOfDrawing->setHorizontalHeaderLabels(headers);

    std::vector<std::shared_ptr<Game>> games = Tournament.getTourGames(Tournament.getCurrentTour());
    int size = games.size();
    for (int i = 0; i < size; ++i)
    {
        ui->tableWidgetOfDrawing->setCellWidget(
            i,0, new QLabel(
                Tournament.getPlayerById(
                                     games[i]->getWhitePlayerId())->getName()));
        if(!(i ==  size-1 && Tournament.getPlayerCount()%2))
        {
            ui->tableWidgetOfDrawing->setCellWidget(
                i,1, new QLabel(
                    Tournament.getPlayerById(
                                     games[i]->getBlackPlayerId())->getName()));
        }
    }


    for(int i = 0; i<rowCount; i++)
    {
        QComboBox * comboBox = new QComboBox();
        comboBox->addItem("-");
        comboBox->addItem("1-0");
        comboBox->addItem("0-1");
        comboBox->addItem("0.5-0.5");


        if(!(Tournament.getCurrentoOganizedTour() == Tournament.getCurrentTour()) && Tournament.hasTheTournamentStarted())
        {

            std::vector<std::shared_ptr<Game>> game = Tournament.getTourGames(Tournament.getCurrentTour());

            switch (game[i]->getResult())
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

            ui->pushButtonOkOfDrawing->setVisible(false);
        }
        else
        {
            ui->tableWidgetOfDrawing->setCellWidget(i, 2, comboBox);
        }
    }


    if(Tournament.getPlayerCount()%2) ui->tableWidgetOfDrawing->cellWidget(rowCount-1, 2)->setDisabled(true);


}

void MainWindow::GivingDataToTable(GameManager &Tournament, int toWichTour)
{
    ui->tableWidgetOfTabel->clear();
    int tourCount = Tournament.getTourCount();

    int playerCount = Tournament.getPlayerCount();

    ui->tableWidgetOfTabel->setRowCount(playerCount);
    ui->tableWidgetOfTabel->setColumnCount(tourCount+2);

    QStringList listoOfColumn;
    for (int i = 0; i < tourCount; ++i)
    {
        listoOfColumn << QString::number(i+1);
    }
    listoOfColumn << "Score";
    listoOfColumn << "Extra Point";

    ui->tableWidgetOfTabel->setHorizontalHeaderLabels(listoOfColumn);

    QStringList listOfRows;
    std::vector<std::shared_ptr<Player>> playerList = currentTournament->getPlayers();
    std::map<int, int>* mapOfIdes = new std::map<int,int>;
    {
        int i =0;
        for (auto& it: playerList)
        {
            listOfRows << QString::number(it->getId()) + ". " + it->getName();
            (*mapOfIdes)[it->getId()] = i++;
        }
        ui->tableWidgetOfTabel->setVerticalHeaderLabels(listOfRows);
    }
    for(int i=1; i<=toWichTour; ++i)
    {
        std::vector<std::shared_ptr<Game>> vectorOgGames = Tournament.getTourGames(i);
        bool odd = false;
        for(auto& it : vectorOgGames)
        {
            int res = it->getResult();
            double score;
            switch (res)
            {
                case 1:
                {
                    score=1;
                    break;
                }
                case -1:
                {
                    score=0;
                    break;
                }
                case 0:
                {
                    score=0.5;
                    break;
                }
                case -2:
                {
                    score = 1;
                    odd = true;
                    break;
                }
            }

            {
                QHBoxLayout *horizLay1 = new QHBoxLayout();
                horizLay1->setContentsMargins(5, 5, 0, 0);
                {
                    QLabel* l1 = new QLabel(QString::number(score));
                    horizLay1->addWidget(l1);
                    QLabel* l2 = new QLabel();
                    l2->setMaximumSize(10,18);
                    horizLay1->addWidget(l2);
                }

                QHBoxLayout *horizLay2 = new QHBoxLayout();
                {
                    QLabel* l2 = new QLabel();
                    l2->setMaximumSize(10,18);
                    horizLay2->addWidget(l2);

                    QLabel *labelOfBlackPlayer = new QLabel(odd?"":QString::number(it->getBlackPlayerId()));
                    labelOfBlackPlayer->setMinimumSize(10,14);
                    labelOfBlackPlayer->setStyleSheet("border: 1px solid black; background-color: white; color: black;");
                    horizLay2->addWidget(labelOfBlackPlayer);
                }
                horizLay2->setContentsMargins(0, 0, 2, 5);

                QVBoxLayout *vertLay = new QVBoxLayout();
                vertLay->setContentsMargins(0, 0, 0, 0); // inner padding
                vertLay->addLayout(horizLay1);
                vertLay->addLayout(horizLay2);

                QFrame *container = new QFrame();
                container->setFrameShape(QFrame::Box);
                container->setFrameShadow(QFrame::Plain);
                container->setStyleSheet(odd?"background-color: gray; color: white;":"color: black;");
                container->setLayout(vertLay);
                container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

                QWidget *wrapper = new QWidget();
                QHBoxLayout *wrapLayout = new QHBoxLayout(wrapper);
                wrapLayout->addStretch();          // left spacing
                wrapLayout->addWidget(container);  // container in center
                wrapLayout->addStretch();          // right spacing
                wrapLayout->setContentsMargins(0, 0, 12, 5);

                QVBoxLayout *outerLayout = new QVBoxLayout();
                outerLayout->addStretch();         // top spacing
                outerLayout->addWidget(wrapper);   // wrapper
                outerLayout->addStretch();         // bottom spacing
                outerLayout->setContentsMargins(0, 0, 0, 0);

                QWidget *finalWrapper = new QWidget();
                finalWrapper->setMinimumSize(60,40);
                finalWrapper->setLayout(outerLayout);

                ui->tableWidgetOfTabel->setCellWidget((*mapOfIdes)[it->getWhitePlayerId()], i-1, finalWrapper);
            }

            if(!odd)
            {
                QHBoxLayout *horizLay1 = new QHBoxLayout();
                horizLay1->setContentsMargins(5, 5, 0, 0);
                {

                    QLabel* l1 = new QLabel(QString::number(1-score));
                    l1->setStyleSheet("color: white");
                    horizLay1->addWidget(l1);
                    QLabel* l2 = new QLabel();
                    l2->setMaximumSize(10,18);
                    horizLay1->addWidget(l2);
                }

                QHBoxLayout *horizLay2 = new QHBoxLayout();
                {
                    QLabel* l2 = new QLabel();
                    l2->setMaximumSize(10,18);
                    horizLay2->addWidget(l2);
                    QLabel *labelOfWhitePlayer = new QLabel(QString::number(it->getWhitePlayerId()));
                    labelOfWhitePlayer->setMinimumSize(10,14);
                    labelOfWhitePlayer->setStyleSheet("border: 1px solid black; background-color: white; color: black;");
                    horizLay2->addWidget(labelOfWhitePlayer);
                }
                horizLay2->setContentsMargins(0, 0, 2, 5);

                QVBoxLayout *vertLay = new QVBoxLayout();
                vertLay->setContentsMargins(0, 0, 0, 0); // inner padding
                vertLay->addLayout(horizLay1);
                vertLay->addLayout(horizLay2);

                QFrame *container = new QFrame();
                container->setFrameShape(QFrame::Box);
                container->setFrameShadow(QFrame::Plain);
                container->setStyleSheet("background-color: black; color: white;");
                container->setLayout(vertLay);
                container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

                QWidget *wrapper = new QWidget();
                QHBoxLayout *wrapLayout = new QHBoxLayout(wrapper);
                wrapLayout->addStretch();          // left spacing
                wrapLayout->addWidget(container);  // container in center
                wrapLayout->addStretch();          // right spacing
                wrapLayout->setContentsMargins(0, 0, 12, 5);

                QVBoxLayout *outerLayout = new QVBoxLayout();
                outerLayout->addStretch();         // top spacing
                outerLayout->addWidget(wrapper);   // wrapper
                outerLayout->addStretch();         // bottom spacing
                outerLayout->setContentsMargins(0, 0, 0, 0);

                QWidget *finalWrapper = new QWidget();
                finalWrapper->setMinimumSize(60,40);
                finalWrapper->setLayout(outerLayout);

                ui->tableWidgetOfTabel->setCellWidget((*mapOfIdes)[it->getBlackPlayerId()], i-1, finalWrapper);
            }
        }
    }

    for(int i=0; i<playerCount; i++)
    {
        QLabel* l1 = new QLabel("0");
        ui->tableWidgetOfTabel->setCellWidget(i,tourCount, l1);
        QLabel* l2 = new QLabel("0");
        ui->tableWidgetOfTabel->setCellWidget(i,tourCount+1,l2);
    }



    for(int i=1; i<=toWichTour; i++)
    {
        std::vector<std::shared_ptr<Game>> vectorOgGames = Tournament.getTourGames(i);

        for(auto& it : vectorOgGames)
        {
            double score = 0;
            int res = it->getResult();
            bool odd = false;
            switch (res)
            {
            case 1:
            {
                score+=1;
                break;
            }
            case -1:
            {
                break;
            }
            case 0:
            {
                score+=0.5;
                break;
            }
            case -2:
            {
                score += 1;
                odd = true;
                break;
            }
            }

            QLabel *wLableOfScore = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getWhitePlayerId()],tourCount));

            QLabel *wLableOfExPoint = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getWhitePlayerId()],tourCount+1));

            if(!odd)
            {
                QLabel *bLableOfScore = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getBlackPlayerId()],tourCount));

                QLabel *bLableOfExPoint = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getBlackPlayerId()],tourCount+1));

                wLableOfExPoint->setText(QString::number(bLableOfScore->text().toDouble()/10 + wLableOfExPoint->text().toDouble()));

                bLableOfExPoint->setText(QString::number(wLableOfScore->text().toDouble()/10 + bLableOfExPoint->text().toDouble()));

                bLableOfScore->setText(QString::number(bLableOfScore->text().toDouble() + (1 - score)));
            }

            wLableOfScore->setText(QString::number(wLableOfScore->text().toDouble()+score));


        }
    }

    ui->tableWidgetOfTabel->resizeRowsToContents();
    ui->tableWidgetOfTabel->resizeColumnsToContents();
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
                on_pushButtonAddName_clicked(currentTournament->getPlayerById(i)->getName());
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

    // 1. Удаляем объект турнира
    vectorOfTournaments.erase(vectorOfTournaments.begin() + index);


    // 2. Удаляем связанную кнопку
    vectorOfRadioButtons.erase(vectorOfRadioButtons.begin() + index);

    // 3. Выключаем кнопки
    ui->pushButtonEdit->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);

    // 4. Удаляем дата из db
    dataBase.removeTournamentAndAllDatasInIt(index);

    // 4. Переиндексация
    int i = 0;
    for (auto& it : vectorOfTournaments) {
        it->setIndexOfTournament(i++);
    }
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

std::pair<int, std::vector<std::pair<int,int>>> MainWindow::findMaxValueWithPairs(
    const std::vector<std::vector<int>>& matrixOfScores, std::vector<int>& participantPlayers, std::vector<int> indices)
{
    size_t n = matrixOfScores.size();
    if (n == 2) {
        // Only one choice possible
        return { matrixOfScores[0][1], { {indices[0], indices[1]} } };
    }

    std::pair<int, std::vector<std::pair<int,int>>> best = {INT_MIN, {}};

    for (int i = 0; i < n-1; ++i) {

        auto erasedMatrix = eraseRowCol(matrixOfScores, n-1, i);

        // Remove those indices from list
        std::vector<int> newIndices;
        for (int k = 0; k < (int)indices.size(); ++k) {
            if (k == n-1 || k == i) continue;
            newIndices.push_back(indices[k]);
        }

        auto subResult = findMaxValueWithPairs(erasedMatrix, participantPlayers, newIndices);
        int totalSum = matrixOfScores[n-1][i] + subResult.first;

        if (totalSum > best.first ) {
            best.first = totalSum;
            best.second.clear();
            best.second.push_back({indices[n-1], indices[i]});
            best.second.insert(best.second.end(), subResult.second.begin(), subResult.second.end());
        }
    }

    return best;
}

void printMatrix( std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            auto v = (val ==  INT_MIN) ? -1 : val;
            std::cout << std::setprecision(5) << v << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
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

    if(currentTournament->getCurrentoOganizedTour() == currentTour)
    {
        ui->pushButtonOkOfDrawing->setVisible(true);
        ui->pushButtonNext->setDisabled(true);
    }

    if(currentTour-1 == currentTournament->getSizeOfGameMap())
    {
        ui->pushButtonOkOfDrawing->setVisible(true);

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
                if (currentTournament->HaveThePlayersMet(i,j) || i == j)
                {
                    v.push_back(INT_MIN);
                }
                else
                {
                    ComfortCoef coef;
                    int gameCoef = coef.gameCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                    int colorCoef = coef.colorCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                    int lastColorCoef = coef.lastColorCoef(currentTournament->getPlayerById(i),currentTournament->getPlayerById(j));
                    v.push_back(gameCoef+colorCoef+lastColorCoef);
                }

            }
            matrix.push_back(v);
        }

        // printMatrix(matrix);

        std::pair<int, std::vector<std::pair<int,int>>> bestVersion = findMaxValueWithPairs(matrix, participantPlayers, vectorOfIndices);

        int count = bestVersion.second.size();
        auto result = bestVersion.second;

        for(int i=0; i<count; ++i)
        {
            int wIndex = participantPlayers[result[i].first];
            int bIndex = participantPlayers[result[i].second];

            if(currentTournament->getPlayerById(participantPlayers[result[i].first])->getLastColor()==1)
            {
                currentTournament->setGame(currentTour, std::make_shared<Game>(wIndex, bIndex));
                dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),
                                    wIndex,bIndex);
            }
            else
            {
                currentTournament->setGame(currentTour, std::make_shared<Game>(bIndex,wIndex));
                dataBase.addNewGame(-2,currentTour,currentTournament->getIndexOfTournament(),
                                    bIndex,wIndex);
            }
            currentTournament->ThePlayerSMet(bIndex, wIndex);
            currentTournament->getPlayerById(wIndex)->addExtraPoint(currentTournament->getPlayerById(bIndex)->getCurrentPoint());
            currentTournament->getPlayerById(bIndex)->addExtraPoint(currentTournament->getPlayerById(wIndex)->getCurrentPoint());
        }

        if(weakestPlayerId != -1)
        {
            currentTournament->setGame(currentTour, std::make_shared<Game>(weakestPlayerId,-1));
            dataBase.addNewGame(-2, currentTour, currentTournament->getIndexOfTournament(), weakestPlayerId, -1);
        }


    }

     ui->tableWidgetOfDrawing->clear();

    GivingDataToDrawing(*currentTournament);

    ui->pushButtonPrevious->setDisabled(false);

}

void MainWindow::on_pushButtonPrevious_clicked()
{
    currentTournament->setCurrentTour((currentTournament->getCurrentTour())-1);

    ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

    if(currentTournament->getCurrentTour()==1) ui->pushButtonPrevious->setDisabled(true);

    ui->pushButtonOkOfDrawing->setVisible(false);
    ui->pushButtonNext->setDisabled(false);

    ui->tableWidgetOfDrawing->clear();
     GivingDataToDrawing(*currentTournament);
}


void MainWindow::on_pushButtonOkOfDrawing_clicked()
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
        ui->tabWidget->setTabEnabled(1,true);
        const std::vector<std::shared_ptr<Game>>& game = currentTournament->getTourGames(currentTournament->getCurrentTour());
        int countOfGames = game.size();
        int countOfPlayers = currentTournament->getPlayerCount();

        currentTournament->setCurrentOganizedTour(currentTournament->getCurrentoOganizedTour()+1);


        for(int i = 0; i<countOfGames; ++i)
        {

            if (QComboBox* combo = qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i, 2)))
            {
                int index = combo->currentIndex();
                    switch (index)
                {
                    case 0:
                    {
                        if(!(currentTournament->getPlayerCount()%2) || i!=countOfGames-1)
                        {
                            QMessageBox::critical(this, "Error", "Insert result!!");
                            ui->tabWidget->setTabEnabled(1,false);
                            currentTournament->setCurrentOganizedTour(currentTournament->getCurrentoOganizedTour()-1);
                            return;
                        }
                        game[i]->setResult(-2);

                        int playerId = game[i]->getWhitePlayerId();
                        std::shared_ptr<Player>& currentPlayer =  currentTournament->getPlayerById(playerId);
                        double GrayPlayerCurrentPoint = currentPlayer->getCurrentPoint()+1;
                        QString name = currentPlayer->getName();

                        int colorCoefOfGray = currentPlayer->getColorCoef();

                        currentPlayer->setCurrentPoint(GrayPlayerCurrentPoint);
                        currentPlayer->setLastColor(-1);

                        dataBase.updatePlayersData(GrayPlayerCurrentPoint, currentPlayer->getExtraPoint() ,colorCoefOfGray,-1,name, currentTournament->getIndexOfTournament(),playerId);

                        dataBase.updateGamesData(playerId,-1, -2, currentTournament->getCurrentTour(), currentTournament->getIndexOfTournament());

                        break;

                    }
                    case 1:
                    {
                        game[i]->setResult(1);
                        int whitePlayerId = game[i]->getWhitePlayerId();

                        std::shared_ptr<Player>&currentWhitePlayer =  currentTournament->getPlayerById(whitePlayerId);
                        int colorCoefOfWhite = currentWhitePlayer->getColorCoef()+1;
                        double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint()+1;
                        QString nameW = currentWhitePlayer->getName();


                        currentWhitePlayer->setCurrentPoint(WhitePlayerCurrentPoint);
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(colorCoefOfWhite);

                        dataBase.updatePlayersData(WhitePlayerCurrentPoint, currentWhitePlayer->getExtraPoint(), colorCoefOfWhite,0,nameW, currentTournament->getIndexOfTournament(),whitePlayerId);


                        int blackPlayerId = game[i]->getBlackPlayerId();
                        std::shared_ptr<Player>& currentBlackPlayer =  currentTournament->getPlayerById(blackPlayerId);
                        int colorCoefOfBlack = currentBlackPlayer->getColorCoef()-1;
                        QString nameB = currentBlackPlayer->getName();
                        double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint();


                        // double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint();
                        // currentBlackPlayer->setCurrentPoint(WhitePlayerCurrentPoint);
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(colorCoefOfBlack);

                        dataBase.updatePlayersData(BlackPlayerCurrentPoint, currentBlackPlayer->getExtraPoint(), colorCoefOfBlack,1,nameB, currentTournament->getIndexOfTournament(),blackPlayerId);

                        dataBase.updateGamesData(whitePlayerId,blackPlayerId, 1, currentTournament->getCurrentTour(), currentTournament->getIndexOfTournament());

                        break;

                    }
                    case 2:
                    {
                        game[i]->setResult(-1);
                        int whitePlayerId = game[i]->getWhitePlayerId();

                        std::shared_ptr<Player>& currentWhitePlayer =  currentTournament->getPlayerById(whitePlayerId);
                        int colorCoefOfWhite = currentWhitePlayer->getColorCoef()+1;
                        QString nameW = currentWhitePlayer->getName();
                        double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint();

                        // double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint();
                        // currentWhitePlayer->setCurrentPoint(WhitePlayerCurrentPoint);
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(colorCoefOfWhite);

                        dataBase.updatePlayersData(WhitePlayerCurrentPoint, currentWhitePlayer->getExtraPoint(), colorCoefOfWhite,0,nameW, currentTournament->getIndexOfTournament(),whitePlayerId);


                        int blackPlayerId = game[i]->getBlackPlayerId();
                        std::shared_ptr<Player>& currentBlackPlayer =  currentTournament->getPlayerById(blackPlayerId);
                        double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint()+1;
                        int colorCoefOfBlack = currentBlackPlayer->getColorCoef()-1;
                        QString nameB = currentBlackPlayer->getName();


                        currentBlackPlayer->setCurrentPoint(BlackPlayerCurrentPoint);
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(colorCoefOfBlack);

                        dataBase.updatePlayersData(BlackPlayerCurrentPoint, currentBlackPlayer->getExtraPoint(),colorCoefOfBlack,1,nameB, currentTournament->getIndexOfTournament(),blackPlayerId);

                        dataBase.updateGamesData(whitePlayerId,blackPlayerId, -1, currentTournament->getCurrentTour(), currentTournament->getIndexOfTournament());

                        break;
                    }

                    case 3:
                    {
                        game[i]->setResult(0);

                        int whitePlayerId = game[i]->getWhitePlayerId();
                        std::shared_ptr<Player>& currentWhitePlayer = currentTournament->getPlayerById(whitePlayerId);
                        QString nameW = currentWhitePlayer->getName();
                        double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint()+ 0.5;
                        int colorCoefOfWhite = currentWhitePlayer->getColorCoef()+1;


                        currentWhitePlayer->setCurrentPoint(currentWhitePlayer->getCurrentPoint());
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(colorCoefOfWhite);

                        dataBase.updatePlayersData(WhitePlayerCurrentPoint,currentWhitePlayer->getExtraPoint(),colorCoefOfWhite,0,nameW, currentTournament->getIndexOfTournament(),whitePlayerId);


                        int blackPlayerId = game[i]->getBlackPlayerId();
                        std::shared_ptr<Player>& currentBlackPlayer = currentTournament->getPlayerById(blackPlayerId);
                        QString nameB = currentBlackPlayer->getName();
                        double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint() + 0.5;
                        int colorCoefOfBlack = currentBlackPlayer->getColorCoef()-1;



                        currentBlackPlayer->setCurrentPoint(currentBlackPlayer->getCurrentPoint());
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(colorCoefOfBlack);

                        dataBase.updatePlayersData(BlackPlayerCurrentPoint,currentBlackPlayer->getExtraPoint(),colorCoefOfBlack,1,nameB, currentTournament->getIndexOfTournament(),blackPlayerId);

                        dataBase.updateGamesData(whitePlayerId,blackPlayerId, 0, currentTournament->getCurrentTour(), currentTournament->getIndexOfTournament());

                        break;
                    }

                    default:
                        qDebug() << "The combo box Id is wrong. ";
                        break;
                }
            }
            else
            {
                qDebug() << "No combo box in this cell. ";
            }
        }

        int currentTour = currentTournament->getCurrentTour();

        for(int i = 0; i<countOfGames; ++i)
        {
            qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i,2))->setDisabled(true);
        }

        ui->pushButtonOkOfDrawing->setVisible(false);

        if(currentTournament->getTourCount()>=2 && currentTour < currentTournament->getTourCount())
            ui->pushButtonNext->setDisabled(false);
        else
            ui->pushButtonNext->setDisabled(true);

        std::shared_ptr<QRadioButton> radioButtonOfTours = std::make_shared<QRadioButton>();
        radioButtonOfTours->setText("Tour " + QString::number(currentTour));
        QObject::connect(radioButtonOfTours.get(), &QRadioButton::clicked, this, [this, currentTour]()
        {
            // currentTournament->setCurrentTour(currentTour);
            GivingDataToTable(*currentTournament, currentTour);
            ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
        });

        // currentTournament->setRadioButtonsOfTabel(radioButtonOfTours);
        mapOfTableRadiobuttons[currentTournament->getIndexOfTournament()].emplace_back(radioButtonOfTours);
        ui->horizontalLayoutOFTorursOfTabel->addWidget(radioButtonOfTours.get());



        ui->checkBoxOfSort->setCheckState(Qt::Unchecked);

        emit radioButtonOfTours->click();


        if(currentTournament->getCurrentoOganizedTour()-1==currentTournament->getTourCount())
        {

            int index = currentTournament->getIndexOfTournament();

            QRadioButton* currentRadioButton = vectorOfRadioButtons[index].get();

            removeWidgetFromLayout(ui->verticalLayoutOfTournamnets, currentRadioButton);

            ui->verticalLayoutOfOldTournaments->addWidget(currentRadioButton);

             currentRadioButton = nullptr;

            ui->pushButtonEdit->setDisabled(true);

        }
    }
    else

    {
        return;
    }
}



