#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <qstackedwidget.h>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLayout>
#include <QRadioButton>
#include <iostream>
#include <QButtonGroup>
#include <QList>
#include <QFrame>
#include"gamemanager.h"
#include "comfortcoef.h"
#include "player.h"
#include "game.h"
#include <iomanip>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectFunction();
    void deleteTournamentDetailes();
    void addPlayersToGameManager(GameManager* gameManager);
    bool isDataComplete();
    void clearLayout(QLayout* layout);
    void GivingDataToDrawing(GameManager* Tournament);
    void GivingDataToTable(GameManager* Tournament, int toWichTour);
    void OkOfDrowing();
    std::pair<int, std::vector<std::pair<int,int>>> findMaxValueWithPairs(
        const std::vector<std::vector<int>>&, std::vector<int>& , std::vector<int>);
    void removeWidgetFromLayout(QLayout* layout, QWidget* widget);
    void loadingDatabaseDatas();

private slots:
    void on_pushButtonAddName_clicked(QString text);

    void on_PushButtonOkOfNewTournamnet_clicked(GameManager*);

    void on_pushButtonEdit_clicked();

    void on_pushButtonDelete_clicked();
    void on_pushButtonNext_clicked();

    void on_pushButtonOkOfDrawing_clicked();

    void on_pushButtonPrevious_clicked();

signals:

    void pushButtonAddName_clicked(QString text);


private:
    Ui::MainWindow *ui;

    std::vector<GameManager*> vectorOfTournaments;
    std::vector<QRadioButton *> vectorOfRadioButtons;
    QButtonGroup * radioGroup;

    GameManager * currentTournament;

    QSqlDatabase db;
};
#endif // MAINWINDOW_H
