#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMessageBox>
#include <QTime>

#include "audioplayerslider.h"

#include "FMOD/inc/fmod.hpp"

enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class AudioPlayer : public QObject
{
	Q_OBJECT

public:
	AudioPlayer(AudioPlayerSlider *slider);

	bool isValid();

	void timerEvent(QTimerEvent *ev);

	void reselectMusic(const QString &path);

signals:
	void sendMessage(MessageType type, QString module, QString message);

	void playedOrPaused(bool played);

	void positionChanged(unsigned int pos);

public slots:
	void playOrPause(bool play);

	void changePosition(unsigned int pos);

private:
	bool m_valid;

	int m_timer;

	AudioPlayerSlider *m_slider;

	unsigned int m_pos;

	FMOD::System *m_fmodSystem;

	FMOD::Sound *m_music;

	FMOD::Channel *m_channel;

	bool m_fmodNotInit;

	float m_volume;

	void __initializeFmodSystem();
};

#endif // AUDIOPLAYER_H
