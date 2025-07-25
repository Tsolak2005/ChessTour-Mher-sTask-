#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <qstackedwidget.h>
#include <QHBoxLayout>

void MainWindow::connectFunction()
{
<<<<<<< HEAD
    //stackedwidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
    QObject::connect(ui->okPushButton, &QPushButton::clicked, this,  [this]() { ui->stackedWidget->setCurrentIndex(2); });

    //stackedwidget_1
    //QObject::connect(ui->drawingPushButton, &QPushButton::clicked, this,  [this]() { ui->stackedWidget_2->setCurrentIndex(0); });
    //ui->stackedWidget_2->setCurrentIndex(0);


=======
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});

    //scrollArea
    ui->scrollArea->setWidgetResizable(true);

    // Create a container widget inside the scroll area
    QWidget* container = new QWidget();
    container->setLayout(ui->verticalLayoutOfNames);
    // ui->verticalLayoutOfNames->addWidget(container);
    ui->scrollArea->setWidget(container);
    // QVBoxLayout* containerLayout = new QVBoxLayout(container);
>>>>>>> d373684247abdb99733615dfc1bf1984213c7d1a
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
<<<<<<< HEAD
    // MainWindow::ui->stackedWidget->insertWidget(1,newLine);
    // ui->stackedWidget->setCurrentWidget(newLine);
    ui->verticalLayoutOfNames->insertWidget(++countOfAddedNames, newLine);
=======
    newLine->setMinimumHeight(10);
    QLabel * newLabel = new QLabel();
    newLabel->setMinimumWidth(15);
    newLabel->setNum(++countOfNames);
    horizontalLayoutOfName->addWidget(newLabel);
    horizontalLayoutOfName->addWidget(newLine);
    ui->verticalLayoutOfNames->addLayout(horizontalLayoutOfName);
>>>>>>> d373684247abdb99733615dfc1bf1984213c7d1a
}
