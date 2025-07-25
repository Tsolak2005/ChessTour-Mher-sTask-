#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <qstackedwidget.h>

void MainWindow::connectFunction()
{
    //stackedwidget
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
    QObject::connect(ui->okPushButton, &QPushButton::clicked, this,  [this]() { ui->stackedWidget->setCurrentIndex(2); });

    //stackedwidget_1
    //QObject::connect(ui->drawingPushButton, &QPushButton::clicked, this,  [this]() { ui->stackedWidget_2->setCurrentIndex(0); });
    //ui->stackedWidget_2->setCurrentIndex(0);


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
    static int countOfAddedNames =0;
    QLineEdit * newLine = new QLineEdit();
    // MainWindow::ui->stackedWidget->insertWidget(1,newLine);
    // ui->stackedWidget->setCurrentWidget(newLine);
    ui->verticalLayoutOfNames->insertWidget(++countOfAddedNames, newLine);
}
