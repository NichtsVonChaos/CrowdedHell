#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "audioplayerslider.h"

class AudioPlayer : QObject
{
	Q_OBJECT

public:
	AudioPlayer(AudioPlayerSlider *slider);

private:
	AudioPlayerSlider *m_slider;
};

#endif // AUDIOPLAYER_H
