#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QObject>
#include <QLineEdit>
#include <qstackedwidget.h>



void MainWindow::connectFunction()
{
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->newTournamnetClickButton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(1);});
    // QObject::connect(ui->cancelPushbutton, &QPushButton::clicked, this, [this]() { ui->stackedWidget->insertWidget(1,new QLineEdit());});
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

