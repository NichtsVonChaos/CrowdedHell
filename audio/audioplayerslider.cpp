#include "audioplayerslider.h"

AudioPlayerSlider::AudioPlayerSlider(QWidget *parent) :
	QSlider(parent), m_audioPlayer(nullptr), m_leftButtonPressing(false)
{
	setMinimum(0);
	setMaximum(1000);
}

void AudioPlayerSlider::setAudioPlayer(AudioPlayer *audioPlayer)
{
	m_audioPlayer = audioPlayer;
}

void AudioPlayerSlider::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		m_audioPlayer->playOrPause(false);
		int pos = int(1000.0 * (double(event->x()) / width()));
		if(pos != sliderPosition())
		{
			setValue(pos);
			m_audioPlayer->changePosition(unsigned((pos/1000.0) * m_audioPlayer->getMusicLength()));
		}
		m_leftButtonPressing = true;
	}
}

void AudioPlayerSlider::mouseMoveEvent(QMouseEvent *event)
{
	if(m_leftButtonPressing)
	{
		int pos = int(1000.0 * (double(event->x()) / width()));
		if(pos != sliderPosition())
		{
			setValue(pos);
			m_audioPlayer->changePosition(unsigned((pos/1000.0) * m_audioPlayer->getMusicLength()));
		}
	}
}

void AudioPlayerSlider::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		m_leftButtonPressing = false;
	}
};

