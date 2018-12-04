#include "audioplayerslider.h"

AudioPlayerSlider::AudioPlayerSlider(QWidget *parent) :
	QSlider(parent)
{

}

void AudioPlayerSlider::paintEvent(QPaintEvent *ev)
{
	QPainter painter(this);

	// 进度条
	QRect processBar(5, height() / 2 - 3, width() - 10, 6);
	QPen pen;  // creates a default pen

	pen.setStyle(Qt::DashDotLine);
	pen.setWidth(3);
	pen.setBrush(Qt::green);
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);

	painter.setPen(pen);
	painter.drawRect(processBar);
}

void AudioPlayerSlider::mousePressEvent(QMouseEvent *ev)
{

}

void AudioPlayerSlider::mouseReleaseEvent(QMouseEvent *ev)
{

}

void AudioPlayerSlider::mouseMoveEvent(QMouseEvent *ev)
{

}
