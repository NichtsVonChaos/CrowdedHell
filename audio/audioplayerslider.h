#ifndef AUDIOPLAYERSLIDER_H
#define AUDIOPLAYERSLIDER_H

#include <QSlider>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

class AudioPlayerSlider : public QSlider
{
	Q_OBJECT
public:
	AudioPlayerSlider(QWidget *parent = Q_NULLPTR);

	void paintEvent(QPaintEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;
};

#endif // AUDIOPLAYERSLIDER_H
