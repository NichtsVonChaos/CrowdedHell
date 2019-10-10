#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) :
    QObject(parent),
    m_fmodSystem(nullptr), m_music(nullptr), m_channel(nullptr),
    m_muted(false), m_position(0u), m_length(0u), m_speed(1.0f)
{
    connect(this, &MusicPlayer::message, logger(), &Logger::message);

    FMOD_RESULT result;

    result = FMOD::System_Create(&m_fmodSystem);
    if (result != FMOD_OK)
    {
        emit message(Logger::Type::Error, "FMOD", tr("Failed to create FMOD system. Error type: %1.").arg(result));
        m_fmodSystem = nullptr;
        return;
    }
    else
        emit message(Logger::Type::Info, "FMOD", tr("FMOD system created."));

    result = m_fmodSystem->init(36, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
    {
        emit message(Logger::Type::Error, "FMOD", tr("Failed to initialize FMOD system. Error type: %1.").arg(result));
        return;
    }
    else
        emit message(Logger::Type::Info, "FMOD", tr("FMOD system initialized."));

    m_timer = QObject::startTimer(5);
}

MusicPlayer::~MusicPlayer()
{
    setPaused(true, this);

    if(m_music != nullptr)
        m_music->release();

    if(m_channel != nullptr)
        delete m_channel;

    if(m_fmodSystem != nullptr)
        m_fmodSystem->release();
}

void MusicPlayer::timerEvent(QTimerEvent *ev)
{
    FMOD_RESULT result;
    if(ev->timerId() == m_timer)
    {
        if(m_channel == nullptr)
        {
            m_position = 0;
            emit positionChanged(0, this);
            return;
        }

        unsigned int lastPosition = m_position;

        result = m_channel->getPosition(&m_position, FMOD_TIMEUNIT_MS);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to get current position. Error type: %1.").arg(result));
            setPaused(true);
            return;
        }



        if(lastPosition != m_position)
        {

        }
    }
}

void MusicPlayer::setVolume(float volume, QObject *sender)
{
    if(sender == options())
    {

    }
    else if(sender == this)
    {

    }
}

void MusicPlayer::setPaused(bool paused, QObject *sender)
{

}

void MusicPlayer::setPosition(unsigned int pos, QObject *sender)
{

}

void MusicPlayer::setSpeed(float speed, QObject *sender)
{

}

float MusicPlayer::speed() const
{
    return m_speed;
}

void MusicPlayer::changeMusic(const QString &path)
{

}

void MusicPlayer::forward(unsigned int frames)
{

}

void MusicPlayer::back(unsigned int frames)
{

}

unsigned int MusicPlayer::length() const
{
    return m_length;
}

unsigned int MusicPlayer::position() const
{
    return m_position;
}

void MusicPlayer::setMuted(bool muted, QObject *sender)
{

}
