#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <qstackedwidget.h>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include"gamemanager.h"
void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});

    // Create a container widget inside the scrollAreaOfPlayersName
    QWidget* container1 = new QWidget();
    container1->setLayout(ui->verticalLayoutOfNames);
    ui->scrollAreaOfPlayersName->setWidget(container1);

    // Create a container widget inside the scrollAreaOfTournaments
    QWidget* container2 = new QWidget();
    container2->setLayout(ui->verticalLayoutOfTournamnets);
    ui->scrollAreaOfTournaments->setWidget(container2);

 }

void MainWindow::deletTournamentDetailes()
 {
    ui->lineEditOfName ->clear();
    ui->lineEditOfData ->clear();
    ui->lineEditOfTourCount ->clear();
    ui->textEdit->clear();
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
        if (!playerName.isEmpty()) {
            Player* newPlayer = new Player();
            newPlayer->setName(playerName);
            gameManager->addNewPlayer(newPlayer);
        }
    }
}

void MainWindow::on_okPushButton_clicked()
{
    if(ui->verticalLayoutOfNames->count() && ui->lineEditOfTourCount->isModified() && ui->lineEditOfData->isModified() && ui->lineEditOfName->isModified() && ui->textEdit->document()->isModified())
    {
        ui->stackedWidget->setCurrentIndex(2);
        QHBoxLayout *horizontalLayoutOfName = new QHBoxLayout();

        QLabel *newLabel = new QLabel();
        // newLabel->setMinimumWidth(15);
        newLabel->setText(ui->lineEditOfName->text());
        QPushButton * buttonOfEdit = new QPushButton();
        buttonOfEdit->setText("Edit");
        QPushButton * buttonOfDelet = new QPushButton();
        buttonOfDelet->setText("Delete");

        horizontalLayoutOfName->addWidget(newLabel);
        horizontalLayoutOfName->addWidget(buttonOfEdit);
        horizontalLayoutOfName->addWidget(buttonOfDelet);

        ui->verticalLayoutOfTournamnets->addLayout(horizontalLayoutOfName);
        ui->tableWidget->setRowCount(ui->verticalLayoutOfNames->count());

        GameManager* Tour = new GameManager();
        Tour->setTourName(ui->lineEditOfName->text());
        Tour->setTourCount(ui->lineEditOfTourCount->text().toInt());
        Tour->setDate(ui->lineEditOfData->text());
        Tour->setPlayerCount(ui->verticalLayoutOfNames->count());
        ui->infoTab->setText(ui->textEdit->toPlainText());

        addPlayersToGameManager(Tour);

       /*     TO ENSURE THAT m_player_list CONTAINS ALL THE PLAYERS,
                PLAYERS' NAMES WERE PRINTED IN INFOTAB

        QString currentText = ui->infoTab->toPlainText();  // or toPlainText() if infoTab is a QTextEdit
        QString newText;

        for (int i = 0; i < Tour->getPlayerCount(); ++i) {
            Player* p = Tour->m_playerList[i];
            if (!p->getName().isEmpty()) {
                if (!newText.isEmpty())
                    newText += ", ";
                newText += p->getName();
            }
        }
        if (!currentText.isEmpty() && !newText.isEmpty())
            currentText += "\n";  // add newline before appending new names

        currentText += newText;
        ui->infoTab->setText(currentText); */
    }
    else
    {
        QMessageBox * message = new QMessageBox();
        message->setText("No Players");
        message->show();
    }


}

