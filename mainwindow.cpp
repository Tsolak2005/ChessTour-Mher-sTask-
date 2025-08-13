#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
        ui->pushButtonOKDrowing->setVisible(true);
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
    ui->pushButtonPrevios->setDisabled(true);
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
    int rowCount = Tournament->getPlayerCount()%2?Tournament->getPlayerCount()/2 +1 : Tournament->getPlayerCount()/2;
    ui->tableWidgetOfDrawing->setRowCount(rowCount);
    ui->infoTab->setText(Tournament->getInfo());
    int countOfPlayers = Tournament->getPlayerCount();
    for(int row = 0, playerId =1  ; playerId <= countOfPlayers; ++row, ++playerId)
    {
        ui->tableWidgetOfDrawing->setCellWidget(row,0,new QLabel(Tournament->getPlayerById(playerId)->getName()));
         if(playerId+1 <= countOfPlayers)
         {
            ui->tableWidgetOfDrawing->setCellWidget(row,1,new QLabel(Tournament->getPlayerById(++playerId)->getName()));
         }
    }

    for(int i = 0; i<rowCount; i++)
    {
        QComboBox * comboBox = new QComboBox();
        comboBox->addItem("-");
        comboBox->addItem("1-0");
        comboBox->addItem("0-1");
        comboBox->addItem("0.5-0.5");
        ui->tableWidgetOfDrawing->setCellWidget(i, 2, comboBox);
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





void MainWindow::on_pushButtonNext_clicked()
{
    currentTournament->setCurrentTour((currentTournament->getCurrentTour())+1);
    if(currentTournament->getCurrentTour() + 1 == currentTournament->getTourCount())
    {
        std::cout << "aaaa";
        ui->pushButtonNext->setDisabled(true);
    }

    ui->labelOfTour->setText("Tour " + QString::number(currentTournament->getCurrentTour()));

    if(!ui->pushButtonNext->isEnabled())
    {
        ui->pushButtonOKDrowing->setVisible(true);
    }
}

void MainWindow::on_pushButtonPrevious_clicked()
{
    currentTournament->setCurrentTour((currentTournament->getCurrentTour())-1);
    if(currentTournament->getCurrentTour()>=2)
    {
        if(currentTournament->getCurrentTour()==2) ui->pushButtonPrevios->setDisabled(true);
        int count = currentTournament->getCurrentTour();
        currentTournament->setCurrentTour(--count);
    }
    ui->pushButtonOKDrowing->setVisible(false);
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
                        int witePlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->getCurrentPoint();
                        currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->setCurrentPoint(++witePlayerCurrentPoint);
                        break;

                    }
                    case 2:
                    {
                        int blackPlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->getCurrentPoint();
                        currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->setCurrentPoint(++blackPlayerCurrentPoint);
                        break;
                    }

                    case 3:
                    {
                        (*game)[i]->setResult(1);
                        int witePlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->getCurrentPoint() + 0.5;
                        currentTournament->getPlayerById((*game)[i]->getWhitePlayerId())->setCurrentPoint(witePlayerCurrentPoint);

                        int blackPlayerCurrentPoint = currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->getCurrentPoint() + 0.5;
                        currentTournament->getPlayerById((*game)[i]->getBlackPlayerId())->setCurrentPoint(blackPlayerCurrentPoint);
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
        ui->pushButtonOKDrowing->setVisible(false);
        if(currentTournament->getTourCount()>=2)ui->pushButtonNext->setDisabled(false);
    }
    else
    {
        return;
    }
}


