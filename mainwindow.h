#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLayout>
#include <QMainWindow>
#include <QRadioButton>
#include"gamemanager.h"
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
    void EditFunction(GameManager* Tournament);
    void OkOfDrowing();

private slots:
    void on_pushButtonAddName_clicked(QString text);

    void on_PushButtonOkOfNewTournamnet_clicked(GameManager*);

    void on_pushButtonEdit_clicked();

    void on_pushButtonDelete_clicked();
    void on_pushButtonNext_clicked();

    void on_pushButtonOKDrowing_clicked();

    void on_pushButtonPrevios_clicked();

signals:

    void pushButtonAddName_clicked(QString text);


private:
    Ui::MainWindow *ui;
    std::vector<GameManager*> vectorOfTournaments;
    std::vector<QRadioButton *> vectorOfRadioButtons;
    GameManager * currentTournament;

};
#endif // MAINWINDOW_H
