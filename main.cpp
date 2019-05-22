#include "crowdedhellgui.h"
#include <QApplication>
#include <iostream>
#include <mainwindow/roomeditor.h>

CrowdedHellGUI *g_mainWindow;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	w.show();
	RoomEditor b(&w);
	b.show();

	return a.exec();
};
