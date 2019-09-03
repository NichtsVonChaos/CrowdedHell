#include "roomeditor.h"
#include "ui_roomeditor.h"

RoomEditor::RoomEditor(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RoomEditor)
{
	ui->setupUi(this);
}

RoomEditor::~RoomEditor()
{
	delete ui;
}
