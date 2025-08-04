#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLayout>
#include <QMainWindow>
#include <QRadioButton>
#include"gamemanager.h"

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
    void DeleteFunction(GameManager* Tournament);

private slots:
    void on_pushButtonAddName_clicked();

    void on_okPushButton_clicked();

    void on_pushButtonEdit_clicked();

    void on_pushButtonDelete_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<GameManager*> vectorOfTournaments;
    std::vector<QRadioButton*> vectorOfRadioButtons;

};
#endif // MAINWINDOW_H
