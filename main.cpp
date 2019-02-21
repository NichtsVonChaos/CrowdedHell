#include "crowdedhellgui.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	w.show();

	return a.exec();
};


int aFunc(int a, int b, std::function<int(int, int)> func)
{
	return func(a, b);
}
