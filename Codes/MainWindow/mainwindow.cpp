#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    project()->initialze();

    m_musicPlayer = new MusicPlayer(this);
    ui->musicSlider->initialze(m_musicPlayer);
}

MainWindow::~MainWindow()
{
	delete ui;
    delete m_musicPlayer;
}
