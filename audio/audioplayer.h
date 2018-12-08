#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMessageBox>
#include <QTime>

#include "audioplayerslider.h"

#include "FMOD/inc/fmod.hpp"

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

/**
 * @brief The AudioPlayer class
 * The audio control center.
 */
class AudioPlayer : public QObject
{
	Q_OBJECT

public:
	AudioPlayer(AudioPlayerSlider *slider, CrowdedHellGUI *parent);

	void timerEvent(QTimerEvent *ev);

	/**
	 * @brief reselectMusic
	 * Reselect music as background music.
	 * @param path
	 * File path.
	 */
	void reselectMusic(const QString &path);

	/**
	 * @brief forward
	 * Let music forward specified frames.
	 */
	void forward(unsigned int frames);

	/**
	 * @brief back
	 * Let music back specified frames.
	 */
	void back(unsigned int frames);

	/**
	 * @brief getPosition
	 * Get playing position of music (miliseconds).
	 */
	unsigned int getPosition();

signals:
	/**
	 * @see CrowdedHellGUI::sendMessage(MessageType, QString, QString);
	 */
	void sendMessage(MessageType type, QString module, QString message);

	/**
	 * @brief playedOrPaused
	 * Emit the signal while music played or paused.
	 */
	void playedOrPaused(bool played);

	/**
	 * @brief positionChanged
	 * Emit the signal while music changed position (milisenconds).
	 */
	void positionChanged(unsigned int pos);

public slots:

	/**
	 * @brief playOrPause
	 * Play or Pause music.
	 */
	void playOrPause(bool play);

	/**
	 * @brief changePosition
	 * Change music position (milisenconds).
	 */
	void changePosition(unsigned int pos);

private:
	bool m_fmodNotInit;

	unsigned int m_pos, m_length;

	int m_timer;

	float m_volume;

	CrowdedHellGUI *m_parent;

	AudioPlayerSlider *m_slider;

	FMOD::System *m_fmodSystem;

	FMOD::Sound *m_music;

	FMOD::Channel *m_channel;

	void __initializeFmodSystem();
};

#endif // AUDIOPLAYER_H
