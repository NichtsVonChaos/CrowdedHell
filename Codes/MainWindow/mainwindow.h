#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Codes/GlobalComponent/logger.h"
#include "Codes/GlobalComponent/optionsmanager.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:

private:
	Ui::MainWindow *ui;

    MusicPlayer *m_musicPlayer;
};

#endif // MAINWINDOW_H
