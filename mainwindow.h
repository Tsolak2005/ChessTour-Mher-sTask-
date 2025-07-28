#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_pushButtonAddName_clicked();

private:
    int countOfNames = 0;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
