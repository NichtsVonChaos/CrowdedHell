#include "crowdedhellgui.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	w.show();

	return a.exec();
}
