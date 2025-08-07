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
<<<<<<< HEAD
    QObject::connect(ui->pushButtonDelete, &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->tableWidgetOfTabel->clear();
=======
    QObject::connect(ui->cancelPushbutton,  &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0); deleteTournamentDetailes();
>>>>>>> 90dc450a439cda694acccf5ec40a73012c0f6953
    });

    //ok and cancel of new tournamnet
    QObject::connect(ui->cancelPushbutton,  &QPushButton::clicked, this, [this]()
    {
        ui->stackedWidget->setCurrentIndex(0);
        deleteTournamentDetailes();
    });

    //Drowing
    ui->infoTab->setReadOnly(true);
    ui->pushButtonNext->setDisabled(true);
    ui->pushButtonPrevios->setDisabled(true);
    ui->tabWidget->setCurrentIndex(0);
     //QObject::connect(ui->pushButtonOKDrowing, &QPushButton::clicked, this, [this](){if(){ui->pushButtonNext->setDisabled(false);}else{} });

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
    group = new QButtonGroup(this);
};

MainWindow::~MainWindow()
{
    delete ui;
    delete group;
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

    for (int i = 0; i < rowCount; ++i) {
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

        QString playerName = lineEdit->text().trimmed();
        if (!playerName.isEmpty()) {
            Player* newPlayer = new Player();
            newPlayer->setName(playerName);
            newPlayer->setColorCoef(0);
            newPlayer->setLastColor(0);
            newPlayer->setCurrentPoint(0);
            newPlayer->setId(i+1);
            gameManager->addNewPlayer(newPlayer);
        }
    }
}

>>bool MainWindow::isDataComplete( )
{
    if(!(ui->verticalLayoutOfNames->count() &&
<<<<<<< HEAD
          !ui->lineEditOfTourCount->text().isEmpty() &&
          !ui->lineEditOfTourCount->text().toInt() &&
          !ui->lineEditOfData->text().isEmpty() &&
          !ui->lineEditOfName->text().isEmpty() &&
          !ui->textEditOfInfo->document()->isEmpty()))
=======
          ui->lineEditOfTourCount->isModified() &&
          ui->lineEditOfTourCount->text().toInt() &&
          ui->lineEditOfData->isModified() &&
          ui->lineEditOfName->isModified() &&
          ui->textEdit->document()->isModified()))
>>>>>>> 90dc450a439cda694acccf5ec40a73012c0f6953
        return false;

    int rowCount = ui->verticalLayoutOfNames->count();

    for (int i = 0; i < rowCount; ++i) {
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

        if(!lineEdit->text().isEmpty()) return false;
    }
    return true;
}

void MainWindow::on_PushButtonOkOfNewTournamnet_clicked()
{
    if(isDataComplete())
    {
        ui->stackedWidget->setCurrentIndex(2);
        QRadioButton * radioButton = new QRadioButton();
        radioButton->setText(ui->lineEditOfName->text());


        ui->verticalLayoutOfTournamnets->addWidget(radioButton);
        vectorOfRadioButtons.push_back(radioButton);
        group->addButton(radioButton);

        ui->lineEditOfTourCount->setDisabled(false);
        ui->pushButtonAddName->setDisabled(false);


        GameManager* Tournament = new GameManager();
        Tournament->setTourName(ui->lineEditOfName->text());
        Tournament->setTourCount(ui->lineEditOfTourCount->text().toInt());
        Tournament->setDate(ui->lineEditOfData->text());
        Tournament->setPlayerCount(ui->verticalLayoutOfNames->count());
        Tournament->setInfo(ui->textEditOfInfo->toPlainText());

        addPlayersToGameManager(Tournament);
        vectorOfTournaments.push_back(Tournament);
<<<<<<< HEAD


        QObject::connect(radioButton, &QRadioButton::clicked, this, [Tournament, this](){
            ui->pushButtonEdit->setDisabled(false);
            ui->pushButtonDelete->setDisabled(false);
            ui->stackedWidget->setCurrentIndex(2);
            int size = vectorOfTournaments.size();
            EditFunction(Tournament);
        });

        emit radioButton->click();

=======
        QObject::connect(radioButton, &QRadioButton::clicked, this, [this](){
            ui->pushButtonEdit->setDisabled(false); ui->pushButtonDelete->setDisabled(false);
        });
        id = 0;
>>>>>>> 90dc450a439cda694acccf5ec40a73012c0f6953
        deleteTournamentDetailes();

    }
    else
    {
        QMessageBox * message = new QMessageBox();
        message->setText("Data isn't complete");
        message->show();
    }
}

void MainWindow::EditFunction(GameManager* Tournament)
{
    int rowCount = Tournament->getPlayerCount()%2?Tournament->getPlayerCount()/2 +1 : Tournament->getPlayerCount()/2;
    ui->tableWidgetOfDrowing->setRowCount(rowCount);
    ui->infoTab->setText(Tournament->getInfo());
    int countOfPlayers = Tournament->getPlayerCount();
    for(int row = 0, playerId =1  ; playerId <= countOfPlayers; ++row, ++playerId)
    {
        ui->tableWidgetOfDrowing->setCellWidget(row,0,new QLabel(Tournament->getPlayerById(playerId)->getName()));
         if(playerId+1 <= countOfPlayers)
         {
            ui->tableWidgetOfDrowing->setCellWidget(row,1,new QLabel(Tournament->getPlayerById(++playerId)->getName()));
         }
    }

    for(int i = 0; i<rowCount; i++)
    {
        QComboBox * comboBox = new QComboBox();
        comboBox->addItem("-");
        comboBox->addItem("1-0");
        comboBox->addItem("0-1");
        comboBox->addItem("0.5-0.5");
        ui->tableWidgetOfDrowing->setCellWidget(i, 2, comboBox);
    }
}


void MainWindow::on_pushButtonEdit_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    const int count = vectorOfRadioButtons.size();
    for(int i = 0; i < count; ++i)
    {
        if (vectorOfRadioButtons[i]->isChecked())
        {
            ui->lineEditOfName->setText(vectorOfTournaments[i]->getTourName());
            ui->lineEditOfData->setText(vectorOfTournaments[i]->getDate());
            ui->textEditOfInfo->setText(vectorOfTournaments[i]->getInfo());
            ui->lineEditOfTourCount->setText(QString::number(vectorOfTournaments[i]->getTourCount()));

            int playerCount = vectorOfTournaments[i]->getPlayerCount();

            for(int j = 1; j<=playerCount; ++j)
            {
                if (vectorOfTournaments[i] && vectorOfTournaments[i]->getPlayerById(j)) {
                    emit pushButtonAddName_clicked(vectorOfTournaments[i]->getPlayerById(j)->getName());
                }
            }

            if(vectorOfTournaments[i]->isTheTournamnetStarted())
            {
                ui->lineEditOfTourCount->setDisabled(true);
                ui->pushButtonAddName->setDisabled(true);
            }
            break;
        }
    }
}


void MainWindow::on_pushButtonDelete_clicked()
{
    int index = 0;
    for ( auto it = vectorOfRadioButtons.begin(); it != vectorOfRadioButtons.end(); ++it, ++index)
    {
        if ((*it)->isChecked())
        {
            delete vectorOfTournaments[index]; // delete the pointer
            vectorOfTournaments.erase(vectorOfTournaments.begin() + index); // remove from tournaments
            vectorOfRadioButtons.erase(it); // remove from radio buttons
            ui->verticalLayoutOfTournamnets->removeItem(ui->verticalLayoutOfTournamnets->takeAt(index));
            ui->pushButtonEdit->setDisabled(true);
            ui->pushButtonDelete->setDisabled(true);

            return;
        }
    }
}

void MainWindow::on_pushButtonNext_clicked()
{


}



