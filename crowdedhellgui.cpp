#include "crowdedhellgui.h"
#include "ui_crowdedhellgui.h"

CrowdedHellGUI::CrowdedHellGUI(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CrowdedHellGUI)
{
	ui->setupUi(this);
}

CrowdedHellGUI::~CrowdedHellGUI()
{
	delete ui;
}
