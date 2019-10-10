#include "MainWindow/mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "ResourceManager/Image/mask.h"
#include <cstring>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
