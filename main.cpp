#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // You could check registry values for system dark/light mode
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
