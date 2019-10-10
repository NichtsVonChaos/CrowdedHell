#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent)
{
    connect(this, &MusicPlayer::message, logger(), &Logger::message);
}

MusicPlayer::~MusicPlayer()
{

}

void MusicPlayer::timerEvent(QTimerEvent *ev)
{

}

void MusicPlayer::setVolume(float volume, QObject *sender)
{
    if(sender == options())
    {

    }
}
