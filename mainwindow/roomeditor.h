#ifndef ROOMEDITOR_H
#define ROOMEDITOR_H

#include <QMainWindow>

namespace Ui {
	class RoomEditor;
}

class RoomEditor : public QMainWindow
{
	Q_OBJECT

public:
	explicit RoomEditor(QWidget *parent = nullptr);
	~RoomEditor();

private:
	Ui::RoomEditor *ui;
};

#endif // ROOMEDITOR_H
