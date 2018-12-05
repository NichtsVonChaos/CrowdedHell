#ifndef CROWDEDHELLGUI_H
#define CROWDEDHELLGUI_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QTranslator>
#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include <QSettings>
#include "display/avoidancedisplaywidget.h"
#include "fmod.hpp"

using namespace FMOD;

namespace Ui {
	class CrowdedHellGUI;
}

class CrowdedHellGUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit CrowdedHellGUI(QWidget *parent = nullptr);
	~CrowdedHellGUI();

	enum class Language
	{
		EN, ZH_CN, ZH_TW, JP
	};

	void changeEvent(QEvent *event);

signals:
	void languageChanged(CrowdedHellGUI::Language);

private slots:
	void on_actionSimplifiedChinese_triggered();

	void on_actionEnglish_triggered();

	void on_actionTraditionalChinese_triggered();

	void on_actionJapanese_triggered();

	void on_actionAddSoundEffect_triggered();

	void on_actionReselectMusic_triggered();

private:
	Ui::CrowdedHellGUI *ui;

	QMap<Language, QTranslator*> m_translators;

	AvoidanceDisplayWidget *m_displayWidget;

	void __updateLanguage(Language language);

	void __readSettings();
};

#endif // CROWDEDHELLGUI_H
