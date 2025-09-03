#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

void MergeSortForPlayers(std::vector<Player*>& Players, int start, int end, std::vector<double>& Scores)
{
    if (end - start < 2) return;

    if (end - start == 2) {
        if (Scores[start] < Scores[start+1]) {
            std::swap(Players[start], Players[start+1]);
            std::swap(Scores[start], Scores[start+1]);
        }
        return;
    }

    int mid = start + (end - start) / 2;

    MergeSortForPlayers(Players, start, mid, Scores);
    MergeSortForPlayers(Players, mid, end, Scores);

    // слияние
    std::vector<Player*> tempPlayers;
    std::vector<double> tempScores;

    tempPlayers.reserve(end - start);
    tempScores.reserve(end - start);

    int i = start;
    int j = mid;

    while (i < mid && j < end)
    {
        if (Scores[i] >= Scores[j]) {
            tempPlayers.push_back(Players[i]);
            tempScores.push_back(Scores[i]);
            i++;
        } else {
            tempPlayers.push_back(Players[j]);
            tempScores.push_back(Scores[j]);
            j++;
        }
    }

    while (i < mid) {
        tempPlayers.push_back(Players[i]);
        tempScores.push_back(Scores[i]);
        i++;
    }
    while (j < end) {
        tempPlayers.push_back(Players[j]);
        tempScores.push_back(Scores[j]);
        j++;
    }

    for (int k = 0; k < (int)tempPlayers.size(); k++) {
        Players[start + k] = tempPlayers[k];
        Scores[start + k] = tempScores[k];
    }
}


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

    // Create a container widget inside the scrollAreaOfOld Tournaments
    QWidget* container3 = new QWidget();
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
    QTabBar* bar = ui->tabWidget->tabBar();

    // получаем кнопку слева у 0-й вкладки
    QPushButton* btn = qobject_cast<QPushButton*>(bar->tabButton(0, QTabBar::LeftSide));

    if (btn) {
        connect(btn, &QPushButton::clicked, this, [=]() {
            ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
        });
    }

    // Sort
    connect(ui->checkBoxOfSort, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state)
    {
        int columnsWithWidgets = 0;
        int columnCount = currentTournament->getCurrentTour();

        for (int col = 0; col < columnCount; ++col) {
            QWidget* w = ui->tableWidgetOfTabel->cellWidget(0, col); // check only row 0
            if (w) {
                ++columnsWithWidgets;
            }
        }

        if(state == Qt::Checked)
        {
            std::vector<Player*> *newPlayerList = new std::vector<Player*>;
            std::vector<Player*>playerList = *currentTournament->getPlayers();
            // if(playerList)
            int playerCount = currentTournament->getPlayerCount();

            for(int i=0; i<playerCount; ++i)
            {
                newPlayerList->push_back(new Player(playerList[i]));
            }

            std::vector<double> Scores;

            int tourCount = currentTournament->getTourCount();
            for(int i=0; i<playerCount; ++i)
            {
                double score = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget(i,tourCount))->text().toDouble();
                Scores.push_back(score);
            }

            MergeSortForPlayers(*newPlayerList, 0, playerCount, Scores);

            currentTournament->changePlayersList(newPlayerList);

            GivingDataToTable(currentTournament, columnsWithWidgets);

            currentTournament->changePlayersList(&playerList);

            delete newPlayerList;

        }
        else
        {
            GivingDataToTable(currentTournament, columnsWithWidgets);
        }
    } );
}

void MainWindow::clearLayout(QLayout* layout)
{
    if (!layout) return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr); // Открепляем от layout, но не удаляем
        }
        else if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);   // Рекурсивно чистим вложенные layout
        }
        delete item; // Удаляем только сам item, не виджет
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
    connectFunction();
    currentTournament = nullptr;

    radioGroup->setExclusive(true);

};

MainWindow::~MainWindow()
{
    // Clean tournaments
    for (auto* t : vectorOfTournaments) {
        delete t; // free GameManager objects
    }
    vectorOfTournaments.clear();

    // Clean radio buttons
    for (auto* rb : vectorOfRadioButtons) {
        delete rb; // free QRadioButton objects
    }
    vectorOfRadioButtons.clear();

    delete radioGroup;

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
            if (!playerName.isEmpty())
            {
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

                ui->pushButtonEdit->setDisabled(false);
                thechangingTournamnet->setTourName(ui->lineEditOfName->text());
                thechangingTournamnet->setDate(ui->lineEditOfData->text());
                thechangingTournamnet->setInfo(ui->textEditOfInfo->toPlainText());
                 int lastPlayerCount = thechangingTournamnet->getPlayerCount();

                addPlayersToGameManager(thechangingTournamnet);
                vectorOfRadioButtons[thechangingTournamnet->getIndexOfTournament()]->setText(ui->lineEditOfName->text());
                ui->stackedWidget->setCurrentIndex(2);
                ui->tableWidgetOfDrawing->clear();

                if(currentTournament->hasTheTournamentStarted())
                {

                }
                else
                {
                }

                if(thechangingTournamnet->hasTheTournamentStarted())
                {
                    ui->labelOfTour->setText("Tour " + QString::number(thechangingTournamnet->getCurrentoOganizedTour()-1));
                    ui->tabWidget->setTabEnabled(1,true);
                    GivingDataToDrawing(thechangingTournamnet);
                    GivingDataToTable(thechangingTournamnet, thechangingTournamnet->getCurrentoOganizedTour()-1);  
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

                            thechangingTournamnet->getTourGames(currentTour)->back()->setBlackPlayerId(++lastPlayerCount);
                        }
                        for(int i=lastPlayerCount+1; i<=playerCount; i++)
                        {
                            Game * newGame = new Game(i);
                            if(i+1<=playerCount)
                            {
                                thechangingTournamnet->ThePlayerSMet(i,i+1);
                                newGame->setBlackPlayerId(++i);
                            }
                           thechangingTournamnet->setGame(currentTour, newGame);
                        }
                    }
                       GivingDataToDrawing(thechangingTournamnet);
                }
                deleteTournamentDetailes();


            }
            else
            {
                ui->labelOfTour->setText("Tour 1");
                ui->tableWidgetOfTabel->clear();
                ui->stackedWidget->setCurrentIndex(2);
                QRadioButton * radioButton = new QRadioButton();
                radioButton->setText(ui->lineEditOfName->text());

                static int countOfRadiobuttons = 0;

                ui->verticalLayoutOfTournamnets->addWidget(radioButton);
                vectorOfRadioButtons.push_back(radioButton);
                radioGroup->addButton(radioButton, countOfRadiobuttons++);

                ui->lineEditOfTourCount->setDisabled(false);
                ui->pushButtonAddName->setDisabled(false);


                GameManager* Tournament = new GameManager(ui->verticalLayoutOfNames->count());
                Tournament->setTourName(ui->lineEditOfName->text());
                Tournament->setTourCount(ui->lineEditOfTourCount->text().toInt());
                Tournament->setDate(ui->lineEditOfData->text());
                Tournament->setInfo(ui->textEditOfInfo->toPlainText());


                addPlayersToGameManager(Tournament);
                vectorOfTournaments.push_back(Tournament);

                int i = 0;
                for (auto* it : vectorOfTournaments) {
                    it->setIndexOfTournament(i++);
                }

                int playerCount = Tournament->getPlayerCount();
                int currentTour = Tournament->getCurrentTour();
                for(int i=1; i<=playerCount; i++)
                {
                    Game * newGame = new Game(i);
                    if(i+1<=playerCount)
                    {
                        Tournament->ThePlayerSMet(i,i+1);
                        newGame->setBlackPlayerId(++i);
                    }
                    Tournament->setGame(currentTour, newGame);
                }

                QObject::connect(radioButton, &QRadioButton::clicked, this, [Tournament, this]()
                {
                    ui->stackedWidget->setCurrentIndex(2);
                    currentTournament = Tournament;

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

                    std::vector<QRadioButton*>* radios = currentTournament->getRadioButtonsOfTabel();
                    for (QRadioButton* rb : *radios) {
                        ui->horizontalLayoutOFTorursOfTabel->addWidget(rb);
                    }

                    GivingDataToTable(currentTournament, currentTournament->getCurrentoOganizedTour()-1);
                    GivingDataToDrawing(Tournament);
                    ui->checkBoxOfSort->setCheckState(Qt::Unchecked);

                    deleteTournamentDetailes();
                });

                emit radioButton->click();
            }
        }
        else
        {
            return;
        }
}






void MainWindow::GivingDataToDrawing(GameManager* Tournament)
{
    if(Tournament->getCurrentoOganizedTour() == Tournament->getCurrentTour()) ui->pushButtonOkOfDrawing->setVisible(true);

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


        if(!(Tournament->getCurrentoOganizedTour() == Tournament->getCurrentTour()) && Tournament->hasTheTournamentStarted())
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

            ui->pushButtonOkOfDrawing->setVisible(false);
            // qobject_cast<QComboBox*>(ui->tableWidgetOfDrawing->cellWidget(i,2))->setDisabled(true);
        }
        else
        {
            ui->tableWidgetOfDrawing->setCellWidget(i, 2, comboBox);
        }
    }

    if(Tournament->getPlayerCount()%2) ui->tableWidgetOfDrawing->cellWidget(rowCount-1, 2)->setDisabled(true);
}


void MainWindow::GivingDataToTable(GameManager *Tournament, int toWichTour)
{
    ui->tableWidgetOfTabel->clear();
    int tourCount = Tournament->getTourCount();
    int playerCount = Tournament->getPlayerCount();
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
    std::vector<Player*>* playerList = currentTournament->getPlayers();
    std::map<int, int>* mapOfIdes = new std::map<int,int>;
    {
        int i =0;
        for (auto* it: *playerList)
        {
            listOfRows << QString::number(it->getId()) + ". " + it->getName();
            (*mapOfIdes)[it->getId()] = i++;
        }
        ui->tableWidgetOfTabel->setVerticalHeaderLabels(listOfRows);
    }
    for(int i=1; i<=toWichTour; ++i)
    {
        std::vector<Game*>* vectorOgGames = Tournament->getTourGames(i);
        bool odd = false;
        for(Game* it : (*vectorOgGames))
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
        QLabel* l = new QLabel();
        ui->tableWidgetOfTabel->setCellWidget(i,tourCount, l);
    }
    for(int i=1; i<=toWichTour; i++)
    {
        std::vector<Game*>* vectorOgGames = Tournament->getTourGames(i);

        for(Game* it : (*vectorOgGames))
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

            QLabel *l1 = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getWhitePlayerId()],tourCount));
            l1->setText(QString::number(l1->text().toDouble()+score));

            if(!odd)
            {
                QLabel *l2 = qobject_cast<QLabel*>(ui->tableWidgetOfTabel->cellWidget((*mapOfIdes)[it->getBlackPlayerId()],tourCount));
                l2->setText(QString::number(l2->text().toDouble()+(1-score)));
            }

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

    // 1. Удаляем объект турнира
    delete vectorOfTournaments[index];
    vectorOfTournaments.erase(vectorOfTournaments.begin() + index);

    // 2. Удаляем связанную кнопку
    delete vectorOfRadioButtons[index];
    vectorOfRadioButtons.erase(vectorOfRadioButtons.begin() + index);

    // 3. Убираем QLayoutItem (только оболочку)
    // QLayoutItem* item = ui->verticalLayoutOfTournamnets->takeAt(index);
    // if (item) {
    //     delete item; // но НЕ удаляем widget, он уже удалён выше
    // }

    // 4. Выключаем кнопки
    ui->pushButtonEdit->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);

    // 5. Переиндексация
    int i = 0;
    for (auto* it : vectorOfTournaments) {
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
                if (currentTournament->HaveThePlayersMet(i,j))
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

        std::pair<int, std::vector<std::pair<int,int>>> bestVersion = findMaxValueWithPairs(matrix, participantPlayers, vectorOfIndices);

        int count = bestVersion.second.size();
        auto result = bestVersion.second;

        for(int i=0; i<count; ++i)
        {
            if(currentTournament->getPlayerById(participantPlayers[result[i].first])->getLastColor()==1)
            {
                currentTournament->setGame(currentTour, new Game(participantPlayers[result[i].first], participantPlayers[result[i].second]));

            }
            else
            {
                currentTournament->setGame(currentTour, new Game(participantPlayers[result[i].second], participantPlayers[result[i].first]));

            }
            currentTournament->ThePlayerSMet(participantPlayers[result[i].second], participantPlayers[result[i].first]);
        }

        if(weakestPlayerId != -1)
            currentTournament->setGame(currentTour, new Game(weakestPlayerId));

    }

     ui->tableWidgetOfDrawing->clear();

    GivingDataToDrawing(currentTournament);

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
     GivingDataToDrawing(currentTournament);
}


void MainWindow::removeWidgetFromLayout(QLayout* layout, QWidget* widget)
{
    if (!layout || !widget) return;

    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (item && item->widget() == widget) {
            layout->takeAt(i);        // убираем item из layout
            widget->setParent(nullptr); // открепляем, но НЕ удаляем
            delete item;              // удаляем сам QLayoutItem
            break;
        }
    }
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
        std::vector<Game*>* game = currentTournament->getTourGames(currentTournament->getCurrentTour());
        int countOfGames = game->size();
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
                        (*game)[i]->setResult(-2);
                        Player* currentPlayer =  currentTournament->getPlayerById((*game)[i]->getWhitePlayerId());
                        double GrayPlayerCurrentPoint = currentPlayer->getCurrentPoint();
                        currentPlayer->setCurrentPoint(++GrayPlayerCurrentPoint);
                        currentPlayer->setLastColor(-2);
                        break;

                    }
                    case 1:
                    {
                        (*game)[i]->setResult(1);
                        Player* currentWhitePlayer =  currentTournament->getPlayerById((*game)[i]->getWhitePlayerId());
                        double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint();
                        currentWhitePlayer->setCurrentPoint(++WhitePlayerCurrentPoint);
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(currentWhitePlayer->getColorCoef()+1);

                        Player* currentBlackPlayer =  currentTournament->getPlayerById((*game)[i]->getBlackPlayerId());
                        // double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint();
                        // currentBlackPlayer->setCurrentPoint(++WhitePlayerCurrentPoint);
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(currentBlackPlayer->getColorCoef()-1);
                        break;

                    }
                    case 2:
                    {
                        (*game)[i]->setResult(-1);
                        Player* currentWhitePlayer =  currentTournament->getPlayerById((*game)[i]->getWhitePlayerId());
                        // double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint();
                        // currentWhitePlayer->setCurrentPoint(++WhitePlayerCurrentPoint);
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(currentWhitePlayer->getColorCoef()+1);

                        Player* currentBlackPlayer =  currentTournament->getPlayerById((*game)[i]->getBlackPlayerId());
                        double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint();
                        currentBlackPlayer->setCurrentPoint(++BlackPlayerCurrentPoint);
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(currentBlackPlayer->getColorCoef()-1);
                        break;
                    }

                    case 3:
                    {
                        (*game)[i]->setResult(0);
                        Player* currentWhitePlayer =  currentTournament->getPlayerById((*game)[i]->getWhitePlayerId());
                        double WhitePlayerCurrentPoint = currentWhitePlayer->getCurrentPoint();
                        currentWhitePlayer->setCurrentPoint(WhitePlayerCurrentPoint + 0.5);
                        currentWhitePlayer->setLastColor(0);
                        currentWhitePlayer->setColorCoef(currentWhitePlayer->getColorCoef()+1);

                        Player* currentBlackPlayer =  currentTournament->getPlayerById((*game)[i]->getBlackPlayerId());
                        double BlackPlayerCurrentPoint = currentBlackPlayer->getCurrentPoint();
                        currentBlackPlayer->setCurrentPoint(BlackPlayerCurrentPoint + 0.5);
                        currentBlackPlayer->setLastColor(1);
                        currentBlackPlayer->setColorCoef(currentBlackPlayer->getColorCoef()-1);
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

        QRadioButton * radioButtonOfTours = new QRadioButton();
        radioButtonOfTours->setText("Tour " + QString::number(currentTour));
        QObject::connect(radioButtonOfTours, &QRadioButton::clicked, this, [radioButtonOfTours, this, currentTour]()
        {
            GivingDataToTable(currentTournament, currentTour);
            ui->checkBoxOfSort->setCheckState(Qt::Unchecked);
        });
        currentTournament->setRadioButtonsOfTabel(radioButtonOfTours);
        ui->horizontalLayoutOFTorursOfTabel->addWidget(radioButtonOfTours);

        // emit ui->checkBoxOfSort->checkStateChanged(Qt::Unchecked);
        ui->checkBoxOfSort->setCheckState(Qt::Unchecked);

        emit radioButtonOfTours->click();

        if(currentTournament->getCurrentoOganizedTour()-1==currentTournament->getTourCount())
        {
            int index = currentTournament->getIndexOfTournament();
            QRadioButton* currentRadioButton = vectorOfRadioButtons[index];

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


