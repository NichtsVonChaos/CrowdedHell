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

class AudioPlayer;

#ifndef AUDIOPLAYER_H
#include "audio/audioplayer.h"
#endif

namespace Ui {
	class CrowdedHellGUI;
}

enum class Language
{
	EN, ZH_CN, ZH_TW, JP
};

enum class MessageType
{
	Info, Error, Warning, Tips
};

class CrowdedHellGUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit CrowdedHellGUI(QWidget *parent = nullptr);
	~CrowdedHellGUI();

	void changeEvent(QEvent *event);

signals:
	void languageChanged(Language language);

	void themeChanged(QString theme);

private slots:
	void on_actionSimplifiedChinese_triggered();

	void on_actionEnglish_triggered();

	void on_actionTraditionalChinese_triggered();

	void on_actionJapanese_triggered();

	void on_actionAddSoundEffect_triggered();

	void on_actionReselectMusic_triggered();

	void sendMessage(MessageType type, QString module, QString message);

private:
	Ui::CrowdedHellGUI *ui;

	QMap<Language, QTranslator*> m_translators;

	AvoidanceDisplayWidget *m_displayWidget;

	AudioPlayer *m_player;

	void __updateLanguage(Language language);

	void __readSettings();
};

#endif // CROWDEDHELLGUI_H
