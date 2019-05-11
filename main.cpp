#include "crowdedhellgui.h"
#include <QApplication>
#include <iostream>

CrowdedHellGUI *g_mainWindow;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	w.show();

	return a.exec();
};
