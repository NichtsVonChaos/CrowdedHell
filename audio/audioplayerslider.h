#ifndef AUDIOPLAYERSLIDER_H
#define AUDIOPLAYERSLIDER_H

#include <QSlider>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

class AudioPlayer;

#ifndef AUDIOPLAYER_H
#include "audioplayer.h"
#endif

class AudioPlayerSlider : public QSlider
{
	Q_OBJECT
public:
	AudioPlayerSlider(QWidget *parent = Q_NULLPTR);

	void setAudioPlayer(AudioPlayer *audioPlayer);

	void mousePressEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	AudioPlayer *m_audioPlayer;

	bool m_leftButtonPressing;

	int __limit(int value, int min, int max);
};

#endif // AUDIOPLAYERSLIDER_H
