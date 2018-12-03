#ifndef AUDIOPLAYERSLIDER_H
#define AUDIOPLAYERSLIDER_H

#include <QSlider>

class AudioPlayerSlider : public QSlider
{
	Q_OBJECT
public:
	AudioPlayerSlider(QWidget *parent = Q_NULLPTR);
};

#endif // AUDIOPLAYERSLIDER_H
