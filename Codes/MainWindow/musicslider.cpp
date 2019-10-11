#include "musicslider.h"

MusicSlider::MusicSlider(QWidget *parent):
    QSlider(parent), m_musicPlayer(nullptr), m_sender(this)
{
    setMaximum(1000);
    connect(this, &MusicSlider::valueChanged, this, &MusicSlider::__valueChanged);
}

MusicSlider::~MusicSlider()
{

}

void MusicSlider::initialze(MusicPlayer *musicPlayer)
{
    m_musicPlayer = musicPlayer;
    connect(m_musicPlayer, &MusicPlayer::positionChanged, this, &MusicSlider::setPosition);
    connect(this, &MusicSlider::positionChanged, m_musicPlayer, &MusicPlayer::setPosition);
}

void MusicSlider::setPosition(unsigned int pos, QObject *sender)
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
