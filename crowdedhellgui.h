#ifndef CROWDEDHELLGUI_H
#define CROWDEDHELLGUI_H

#include <QMainWindow>
#include <QMap>
#include <QTranslator>
#include <QDebug>
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

	void changeEvent(QEvent *event);

private slots:
	void on_actionSimplifiedChinese_triggered();

	void on_actionEnglish_triggered();

	void on_actionTraditionalChinese_triggered();

	void on_actionJapanese_triggered();

private:
	Ui::CrowdedHellGUI *ui;

	enum class Language
	{
		EN, ZH_CN, ZH_TW, JP
	};

	QMap<Language, QTranslator*> m_translators;
	QMap<Language, QAction*> m_languageActions;

	AvoidanceDisplayWidget *m_displayWidget;

	void __refreshTranslation(Language language);
};

#endif // CROWDEDHELLGUI_H
