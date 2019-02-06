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

class AvoidanceDisplayWidget;
#ifndef AVOIDANCEDISPLAYWIDGET_H
#include "display/avoidancedisplaywidget.h"
#endif

class AudioPlayer;

#ifndef AUDIOPLAYER_H
#include "audio/audioplayer.h"
#endif

class ProjectManager;
#ifndef PROJECTMANAGER_H
#include "manager/projectmanager.h"
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

	void musicInvalid();

	void updateMusicLength(unsigned int miliseconds);

	void changeEvent(QEvent *event);

	QString currentTheme();

	Language currentLanguage();

signals:
	void languageChanged(Language language);

	void themeChanged(QString theme);

public slots:
	void musicPositionChanged(unsigned int miliseconds);

	void musicPlayedOrPaused(bool play);

	void sendMessage(MessageType type, QString module, QString message);

	void volumeBarValueChanged(int value);

	void changeMusic(QString musicPath);

	void projectClosed();

private slots:
	void on_actionSimplifiedChinese_triggered();

	void on_actionEnglish_triggered();

	void on_actionTraditionalChinese_triggered();

	void on_actionJapanese_triggered();

	void on_actionAddSoundEffect_triggered();

	void on_actionReselectMusic_triggered();

	void on_buttonPause_toggled(bool checked);

	void on_buttonNextFrame_clicked();

	void on_buttonPrevFrame_clicked();

	void on_buttonNext10Frames_clicked();

	void on_buttonPrev10Frames_clicked();

	void on_buttonNext50Frames_clicked();

	void on_buttonPrev50Frames_clicked();

	void on_lineEditFrames_editingFinished();

	void on_menuTheme_hovered(QAction *action);

	void on_actionNewProject_triggered();

	void on_comboBoxSpeed_currentIndexChanged(int index);

	void on_actionHideInfo_changed();

	void on_buttonMute_toggled(bool checked);

	void on_buttonToZero_pressed();

	void on_lineEditTime_editingFinished();

	void on_actionAlwaysSave_changed();

	void on_actionOpenProject_triggered();

	void on_actionClearMessages_triggered();

private:
	Ui::CrowdedHellGUI *ui;

	QMap<Language, QTranslator*> m_translators;

	Language m_currentLanguage;

	AvoidanceDisplayWidget *m_displayWidget;

	AudioPlayer *m_audioPlayer;

	ProjectManager *m_projectManager;

	QString m_currentMusicName;

    QStringList m_themes;

	QString m_currentTheme;

	void __updateUi();

	void __readSettings();

	void __updateSettings();

	void __turnToDefalutTheme();
};

#endif // CROWDEDHELLGUI_H
