#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <qstackedwidget.h>
#include <QHBoxLayout>

void MainWindow::connectFunction()
{
    //stackedWidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
    QObject::connect(ui->okPushButton, &QPushButton::clicked, this, [this]() { if(ui->verticalLayoutOfNames->count()) ui->stackedWidget->setCurrentIndex(2);});


    //scrollArea
    ui->scrollArea->setWidgetResizable(true);

    // Create a container widget inside the scroll area
    QWidget* container = new QWidget();
    container->setLayout(ui->verticalLayoutOfNames);
    ui->scrollArea->setWidget(container);
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
    newLabel->setNum(++countOfNames);

    horizontalLayoutOfName->addWidget(newLabel);
    horizontalLayoutOfName->addWidget(newLine);

    ui->verticalLayoutOfNames->addLayout(horizontalLayoutOfName);
    ui->tableWidget->setRowCount(countOfNames);
}
