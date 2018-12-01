#ifndef CROWDEDHELLGUI_H
#define CROWDEDHELLGUI_H

#include <QMainWindow>

namespace Ui {
	class CrowdedHellGUI;
}

class CrowdedHellGUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit CrowdedHellGUI(QWidget *parent = nullptr);
	~CrowdedHellGUI();

private:
	Ui::CrowdedHellGUI *ui;
};

#endif // CROWDEDHELLGUI_H
