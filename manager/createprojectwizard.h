#ifndef CREATEPROJECTWIZARD_H
#define CREATEPROJECTWIZARD_H

#include <QWizard>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

namespace Ui {
	class CreateProjectWizard;
}

class CreateProjectWizard : public QWizard
{
	Q_OBJECT

public:
	explicit CreateProjectWizard(CrowdedHellGUI *parent = nullptr);
	~CreateProjectWizard();

	QString getProjectName();
	QString getPath();
	QString getAuthor();
	QDateTime getDate();

private slots:

	void on_toolButtonSelectPath_clicked();

private:
	Ui::CreateProjectWizard *ui;

	QDateTime m_createDate;
};

#endif // CREATEPROJECTWIZARD_H
