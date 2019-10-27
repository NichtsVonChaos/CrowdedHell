#include "MainWindow/mainwindow.h"
#include <QApplication>
#include <singleapplication.h>

int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, &SingleApplication::instanceStarted, &w, &QMainWindow::raise);
    w.show();

    return a.exec();
}
