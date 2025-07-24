#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <qstackedwidget.h>

// void MainWindow::connectFunction()
// {
//     ui->stackedWidget->setCurrentIndex(0);
//     QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
//     QObject::connect(ui->pushButton_addNewName, &QPushButton::clicked, this, [this]() { ui->stackedWidget->insertWidget(1,QEdit);});
// }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // connectFunction();
};

MainWindow::~MainWindow()
{
    delete ui;
}
