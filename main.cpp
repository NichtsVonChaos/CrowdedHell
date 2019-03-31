#include "crowdedhellgui.h"
#include <QApplication>
#include <iostream>

CrowdedHellGUI *g_mainWindow;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	g_mainWindow = &w;
	w.show();

	return a.exec();
};
