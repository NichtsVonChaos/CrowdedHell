#include "crowdedhellgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CrowdedHellGUI w;
	w.show();

	return a.exec();
}
