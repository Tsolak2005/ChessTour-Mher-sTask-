#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <qstackedwidget.h>
#include <QHBoxLayout>
#include <qcheckbox.h>

void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
    QObject::connect(ui->okPushButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(2);});


    //scrollAreaOfPlayerNames
    ui->scrollAreaOfPlayersName->setWidgetResizable(true);

    //scrollAreaOfTournamnets
    ui->scrollAreaOfTournaments->setWidgetResizable(true);



    // Create a container widget inside the scroll area for scrollAreaOfPlayerNames
    QWidget* container1 = new QWidget();
    container1->setLayout(ui->verticalLayoutOfNames);
    ui->scrollAreaOfPlayersName->setWidget(container1);

    // Create a container widget inside the scroll area for scrollAreaOfTournamnets
    QWidget* container2 = new QWidget();
    container2->setLayout(ui->verticalLayoutOfTournamnets);
    ui->scrollAreaOfTournaments->setWidget(container2);

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
    static int countOfNames = 0;
    QHBoxLayout * horizontalLayoutOfName = new QHBoxLayout();
    QLineEdit * newLine = new QLineEdit();
    newLine->setMinimumHeight(10);
    QLabel * newLabel = new QLabel();
    newLabel->setMinimumWidth(15);
    newLabel->setNum(++countOfNames);
    horizontalLayoutOfName->addWidget(newLabel);
    horizontalLayoutOfName->addWidget(newLine);
    ui->verticalLayoutOfNames->addLayout(horizontalLayoutOfName);
}

void MainWindow::on_editPushBotton_clicked()
{
    static int countOfNames = 0;

    QCheckBox * checkbox = new QCheckBox("Tournament " + QString::number(++countOfNames));
    ui->verticalLayoutOfTournamnets->addWidget(checkbox);
}

