#ifndef ADDRESOUCEWIZARD_H
#define ADDRESOUCEWIZARD_H

#include <QWizard>
#include <QFileDialog>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

namespace Ui {
	class AddResourceWizard;
}

class AddResourceWizard : public QWizard
{
	Q_OBJECT

	enum class Type
	{
		SPRITE, SOUND, BACKGROUND
	};

public:
	explicit AddResourceWizard(Type resourceType, QWidget *parent = nullptr, const QString &group = QString());
	~AddResourceWizard();

	Type getType() const;

	QString getFilePath();

	QString getName();

	QString getGroup();

private slots:
	void on_toolButtonFile_clicked();

private:
	Ui::AddResourceWizard *ui;

	Type m_type;
};

#endif // ADDSPRITEWIZARD_H
