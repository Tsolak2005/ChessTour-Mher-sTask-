#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <qstackedwidget.h>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLayout>
#include <QRadioButton>
#include"gamemanager.h"

void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(2);});

    // Create a container widget inside the scrollAreaOfPlayersName
    QWidget* container1 = new QWidget();
    container1->setLayout(ui->verticalLayoutOfNames);
    ui->scrollAreaOfPlayersName->setWidget(container1);

    // Create a container widget inside the scrollAreaOfTournaments
    QWidget* container2 = new QWidget();
    container2->setLayout(ui->verticalLayoutOfTournamnets);
    ui->scrollAreaOfTournaments->setWidget(container2);

    ui->pushButtonEdit->setDisabled(true);
    ui->pushButtonDelete->setDisabled(true);
    QObject::connect(ui->cancelPushbutton,  &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(0); deleteTournamentDetailes();});

 }

void MainWindow::clearLayout(QLayout* layout) {
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
    ui->textEdit->clear();
    clearLayout(ui->verticalLayoutOfNames);

 }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectFunction();
};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAddName_clicked()
{
    QHBoxLayout *horizontalLayoutOfName = new QHBoxLayout();
    QLineEdit *newLine = new QLineEdit();
    newLine->setMinimumHeight(10);

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
        int id = 0;
        if (!playerName.isEmpty()) {
            Player* newPlayer = new Player();
            newPlayer->setName(playerName);
            newPlayer->setColorCoef(0);
            newPlayer->setLastColor(0);
            newPlayer->setCurrentPoint(0);
            newPlayer->setId(++id);
            gameManager->addNewPlayer(newPlayer);
        }
    }
}

bool MainWindow::isDataComplete()
{
    if(!(ui->verticalLayoutOfNames->count() && ui->lineEditOfTourCount->isModified() && ui->lineEditOfData->isModified() && ui->lineEditOfName->isModified() && ui->textEdit->document()->isModified()))
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

        if(!lineEdit->isModified()) return false;
    }
    return true;
}

void MainWindow::on_okPushButton_clicked()
{
    if(isDataComplete())
    {
        ui->stackedWidget->setCurrentIndex(0);
        QRadioButton * radioButton = new QRadioButton();
        radioButton->setText(ui->lineEditOfName->text());

        ui->verticalLayoutOfTournamnets->addWidget(radioButton);
        vectorOfRadioButtons.push_back(radioButton);




        GameManager* Tournament = new GameManager();
        Tournament->setTourName(ui->lineEditOfName->text());
        Tournament->setTourCount(ui->lineEditOfTourCount->text().toInt());
        Tournament->setDate(ui->lineEditOfData->text());
        Tournament->setPlayerCount(ui->verticalLayoutOfNames->count());
        Tournament->setInfo(ui->textEdit->toPlainText());


        addPlayersToGameManager(Tournament);
        vectorOfTournaments.push_back(Tournament);

        QObject::connect(radioButton, &QRadioButton::clicked, this, [this](){ui->pushButtonEdit->setDisabled(false); ui->pushButtonDelete->setDisabled(false); });

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
     ui->tableWidgetOfDrowing->setRowCount(Tournament->getPlayerCount());
     ui->infoTab->setText(Tournament->getInfo());
     // int countOfPlayers = Tournament->getPlayerCount();
     // for(int i = 0; i<3; i++)
     //     for(int j = 0; j<countOfPlayers; j++)
     //     {
     //         ui->tableWidgetOfDrowing
     //     }
     // ui->tableWidgetOfDrowing->set
}


void MainWindow::on_pushButtonEdit_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    int count = vectorOfRadioButtons.size();
    for(int i = 0; i<count; ++i)
    {
        if(vectorOfRadioButtons[i]->isChecked()) EditFunction(vectorOfTournaments[i]);
    }
}

