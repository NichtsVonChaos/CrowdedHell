#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    m_musicPlayer = new MusicPlayer(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}
