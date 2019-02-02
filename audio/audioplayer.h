#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMessageBox>
#include <QTime>

class CrowdedHellGUI;
class AudioPlayerSlider;
enum class MessageType;
enum class Language;

#include "FMOD/inc/fmod.hpp"

#ifndef AUDIOPLAYERSLIDER_H
#include "audioplayerslider.h"
#endif

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

	~AudioPlayer();

	/**
	 * @brief timerEvent
	 * Update time of music's playing per 5 miliseconds.
	 */
	void timerEvent(QTimerEvent *ev);

	/**
	 * @brief reselectMusic
	 * Reselect music as background music.
	 * @param path
	 * File path.
	 */
	void changeMusic(const QString &path);

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

	/**
	 * @brief getMusicLength
	 * Get music length (miliseconds).
	 */
	unsigned int getMusicLength();

	/**
	 * @brief getSpeed
	 * Get music speed.
	 */
	float getSpeed();

	/**
	 * @brief getvolume
	 * Get music volume.
	 */
	float getVolume();

	/**
	 * @brief isPlaying
	 * Return if music is playing.
	 */
	bool isPlaying();

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

	/**
	 * @brief speedChanged
	 * Emit the signal while music changed speed.
	 */
	void speedChanged(float speed);

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

	/**
	 * @brief changeSpeed
	 * Change music speed.
	 */
	void changeSpeed(float speed);

	/**
	 * @brief changeVolume
	 * Change music volume.
	 */
	void changeVolume(float volume);

	/**
	 * @brief mute
	 * Mute or not.
	 */
	void mute(bool mute);

private:
	bool m_fmodNotInit;

	bool m_muted;

	unsigned int m_pos, m_length;

	int m_timer;

	float m_speed;

	float m_volume;

	CrowdedHellGUI *m_parent;

	AudioPlayerSlider *m_slider;

	FMOD::System *m_fmodSystem;

	FMOD::Sound *m_music;

	FMOD::Channel *m_channel;

	void __initializeFmodSystem();

	void __readSettings();

	void __updateSettings();
};

#endif // AUDIOPLAYER_H
