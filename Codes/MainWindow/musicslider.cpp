#include "musicslider.h"

MusicSlider::MusicSlider(QWidget *parent):
    QSlider(parent), m_musicPlayer(nullptr), m_sender(this), m_leftButtonPressing(false)
{
    setMaximum(1000);
    connect(this, &MusicSlider::valueChanged, this, &MusicSlider::__valueChanged, Qt::UniqueConnection);
}

MusicSlider::~MusicSlider()
{

}

void MusicSlider::initialze(MusicPlayer *musicPlayer)
{
    m_musicPlayer = musicPlayer;
    connect(m_musicPlayer, &MusicPlayer::positionChanged, this, &MusicSlider::setPosition, Qt::UniqueConnection);
    connect(this, &MusicSlider::positionChanged, m_musicPlayer, &MusicPlayer::setPosition, Qt::UniqueConnection);
}

void MusicSlider::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        disconnect(m_musicPlayer, &MusicPlayer::positionChanged, this, &MusicSlider::setPosition);
        disconnect(this, &MusicSlider::positionChanged, m_musicPlayer, &MusicPlayer::setPosition);
        int pos = limit(int(1000.0 * (double(event->x()) / width())), 0, 1000);
        setValue(pos);
        m_leftButtonPressing = true;
        event->accept();
    }
}

void MusicSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_leftButtonPressing)
    {
        int pos = limit(int(1000.0 * (double(event->x()) / width())), 0, 1000);
        setValue(pos);
        event->accept();
    }
}

void MusicSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int pos = limit(int(1000.0 * (double(event->x()) / width())), 0, 1000);
        connect(m_musicPlayer, &MusicPlayer::positionChanged, this, &MusicSlider::setPosition, Qt::UniqueConnection);
        connect(this, &MusicSlider::positionChanged, m_musicPlayer, &MusicPlayer::setPosition, Qt::UniqueConnection);
        setValue(pos);
        __valueChanged(pos);
        m_leftButtonPressing = false;
        event->accept();
    }
}

void MusicSlider::setPosition(unsigned int pos, const QObject *sender)
{
    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(m_musicPlayer)
    {
        m_sender = sender;
        if(m_musicPlayer->length())
            setValue(int(pos * 1000 / m_musicPlayer->length()));
        else
            setValue(0);
    }
}

void MusicSlider::__valueChanged(int value)
{
    if(m_musicPlayer)
    {
        if(m_musicPlayer->length())
        {
            unsigned int pos = unsigned(value) * m_musicPlayer->length() / 1000u;
            emit positionChanged(pos, m_sender);
        }
        else if(value != 0)
            emit positionChanged(0, m_sender);

        m_sender = this;
    }
}
