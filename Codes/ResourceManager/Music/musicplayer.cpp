#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) :
    QObject(parent),
    m_fmodSystem(nullptr), m_music(nullptr), m_channel(nullptr),
    m_muted(false), m_position(0u), m_length(0u), m_speed(1.0f),
    m_timerReportError(false)
{
    connect(this, &MusicPlayer::message, logger(), &Logger::message, Qt::UniqueConnection);
    connect(options(), &OptionsManager::volumeChanged, this, &MusicPlayer::setVolume, Qt::UniqueConnection);

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

    if(m_fmodSystem != nullptr)
        m_fmodSystem->release();
}

void MusicPlayer::timerEvent(QTimerEvent *ev)
{
    FMOD_RESULT result;
    if(ev->timerId() == m_timer)
    {
        if(!m_music)
            return;

        if(!m_channel)
        {
            m_position = 0;
            emit positionChanged(0, this);
            m_timerReportError = false;
            return;
        }

        bool isPlaying = false;
        result = m_channel->isPlaying(&isPlaying);
        if(result != FMOD_OK || !isPlaying)
        {
            emit paused(true, this);
            if(m_position != m_length)
            {
                m_position = m_length;
                emit positionChanged(m_length, this);
            }
            m_timerReportError = false;
            return;
        }

        unsigned int lastPosition = m_position;

        result = m_channel->getPosition(&m_position, FMOD_TIMEUNIT_MS);
        if(result != FMOD_OK && !m_timerReportError)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to get current position. Error type: %1.").arg(result));
            m_timerReportError = true;
            return;
        }

        if(lastPosition != m_position)
            emit positionChanged(m_position, this);

        m_timerReportError = false;
    }
}

void MusicPlayer::setVolume(float volume, const QObject *sender)
{
    FMOD_RESULT result;

    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(m_channel == nullptr)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Set volume to %1.").arg(double(options()->volume())));
        return;
    }

    bool isPlaying = false;
    result = m_channel->isPlaying(&isPlaying);
    if(result != FMOD_OK || !isPlaying)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Set volume to %1.").arg(double(options()->volume())));
        return;
    }

    result = m_channel->setVolume(volume);
    if(result != FMOD_OK)
        emit message(Logger::Type::Warning, "FMOD", tr("Failed to set volume to %1. Error type: %2.").arg(double(options()->volume())).arg(result));
    else
        emit message(Logger::Type::Info, "FMOD", tr("Set volume to %1.").arg(double(options()->volume())));
}

void MusicPlayer::pause()
{
    setPaused(true);
}

void MusicPlayer::setPaused(bool paused, const QObject *sender)
{
    FMOD_RESULT result;

    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(paused)
    {
        if(!m_music || !m_channel)
            return;

        bool isPlaying = false;
        result = m_channel->isPlaying(&isPlaying);

        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Warning, "FMOD", tr("Cannot access the channel. It will be regarded as null. Error type: %1.").arg(result));
            return;
        }

        if(!isPlaying)
            return;

        bool isPaused = true;
        result = m_channel->getPaused(&isPaused);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to get the music status. Error type: %1.").arg(result));
            return;
        }
        if(isPaused)
            return;

        result = m_channel->setPaused(true);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to pause music. Error type: %1.").arg(result));
            emit this->paused(false, this);
            return;
        }

        emit message(Logger::Type::Info, "FMOD", tr("Music paused."));
    }
    else
    {
        if(!m_music)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Please create or open a project first."));
            emit this->paused(true, this);
            return;
        }

        if(!m_channel)
        {
            result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
            if(result != FMOD_OK)
            {
                emit message(Logger::Type::Error, "FMOD", tr("Failed to create channel. Error type: %1.").arg(result));
                emit this->paused(true, this);
                emit positionChanged(0, this);
                return;
            }

            result = m_channel->setPitch(m_speed);
            if(result != FMOD_OK)
                emit message(Logger::Type::Warning, "FMOD", tr("Failed to set music speed to x%1. Error type: %2.").arg(QString::number(double(m_speed), 'g', 2)).arg(result));
        }

        bool isPlaying = false;
        result = m_channel->isPlaying(&isPlaying);
        if(result != FMOD_OK || !isPlaying)
        {
            if(result != FMOD_OK)
                emit message(Logger::Type::Warning, "FMOD", tr("Cannot access the channel. It will be reloaded. Error type: %1.").arg(result));

            m_channel = nullptr;
            result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
            if(result != FMOD_OK)
            {
                emit message(Logger::Type::Error, "FMOD", tr("Failed to create channel. Error type: %1.").arg(result));
                emit this->paused(true, this);
                emit positionChanged(0, this);
                return;
            }

            result = m_channel->setPitch(m_speed);
            if(result != FMOD_OK)
                emit message(Logger::Type::Warning, "FMOD", tr("Failed to set music speed to x%1. Error type: %2.").arg(QString::number(double(m_speed), 'g', 2)).arg(result));
        }

        if(m_position >= m_length)
        {
            emit message(Logger::Type::Warning, "FMOD", tr("Music is already at the end, replay the music."));
            m_position = 0;
            emit positionChanged(0, this);
        }

        bool isPaused = true;
        result = m_channel->getPaused(&isPaused);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to get the music status. Error type: %1.").arg(result));
            return;
        }
        if(!isPaused)
            return;

        result = m_channel->setPaused(false);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to play music. Error type: %1.").arg(result));
            emit this->paused(true, this);
            return;
        }

        result = m_channel->setVolume(options()->volume());
        if(result != FMOD_OK)
            emit message(Logger::Type::Warning, "FMOD", tr("Failed to set volume to %1. Error type: %2.").arg(double(options()->volume())).arg(result));

        result = m_channel->setMute(m_muted);
        if(result != FMOD_OK)
            emit message(Logger::Type::Warning, "FMOD", tr("Failed to %1 music. Error type: %2.").arg(m_muted ? tr("mute") : tr("unmute")).arg(result));

        emit message(Logger::Type::Info, "FMOD", tr("Music resumed."));
    }

    emit this->paused(paused, sender);
}

void MusicPlayer::setPosition(unsigned int pos, const QObject *sender)
{
    FMOD_RESULT result;

    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(!m_music)
    {
        emit message(Logger::Type::Error, "FMOD", tr("Please create or open a project first."));
        emit positionChanged(0, this);
        return;
    }

    if(!m_channel)
    {
        result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to create channel. Error type: %1.").arg(result));
            emit positionChanged(0, this);
            return;
        }

        result = m_channel->setPitch(m_speed);
        if(result != FMOD_OK)
            emit message(Logger::Type::Warning, "FMOD", tr("Failed to set music speed to x%1. Error type: %2.").arg(QString::number(double(m_speed), 'g', 2)).arg(result));
    }

    bool isPlaying = false;
    result = m_channel->isPlaying(&isPlaying);
    if(result != FMOD_OK || !isPlaying)
    {
        if(result != FMOD_OK)
            emit message(Logger::Type::Warning, "FMOD", tr("Cannot access the channel. It will be reloaded. Error type: %1.").arg(result));

        m_channel = nullptr;
        result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to create channel. Error type: %1.").arg(result));
            emit positionChanged(0, this);
            return;
        }

        result = m_channel->setPitch(m_speed);
        if(result != FMOD_OK)
            emit message(Logger::Type::Warning, "FMOD", tr("Failed to set music speed to x%1. Error type: %2.").arg(QString::number(double(m_speed), 'g', 2)).arg(result));
    }

    if(pos >= m_length)
    {
        emit message(Logger::Type::Warning, "FMOD", tr("Position \"%1 ms\" is equal or larger than music length \"%2 ms\", it will set the position to the end of music.").arg(QString::number(pos), QString::number(m_length)));
        m_position = m_length;
        result = m_channel->setPosition(m_length, FMOD_TIMEUNIT_MS);
        if(result != FMOD_OK)
        {
            emit message(Logger::Type::Error, "FMOD", tr("Failed to set position to \"%1 ms\" of \"%2 ms\".").arg(QString::number(m_position), QString::number(m_length)));
            m_position = 0;
            m_channel->setPaused(true);
            emit positionChanged(0, this);
            emit paused(true, this);
            return;
        }

        result = m_channel->setPaused(true);
        emit positionChanged(m_position, this);
        emit paused(true, this);
        return;
    }

    m_position = pos;
    result = m_channel->setPosition(m_position, FMOD_TIMEUNIT_MS);
    if(result != FMOD_OK)
    {
        emit message(Logger::Type::Error, "FMOD", tr("Failed to set position to \"%1 ms\" of \"%2 ms\".").arg(QString::number(m_position), QString::number(m_length)));
        m_position = 0;
        m_channel->setPaused(true);
        emit positionChanged(0, this);
        emit paused(true, this);
        return;
    }

    emit message(Logger::Type::Info, "FMOD", tr("Set position to \"%1 ms\" of \"%2 ms\".").arg(QString::number(m_position), QString::number(m_length)));
    emit positionChanged(m_position, sender);
}

void MusicPlayer::setSpeed(float speed, const QObject *sender)
{
    FMOD_RESULT result;

    m_speed = speed;

    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(m_channel == nullptr)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Set speed to %1.").arg(double(m_speed)));
        emit speedChanged(m_speed, sender);
        return;
    }

    bool isPlaying = false;
    result = m_channel->isPlaying(&isPlaying);
    if(result != FMOD_OK || !isPlaying)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Set speed to %1.").arg(double(m_speed)));
        emit speedChanged(m_speed, sender);
        return;
    }

    result = m_channel->setPitch(m_speed);
    if(result != FMOD_OK)
        emit message(Logger::Type::Warning, "FMOD", tr("Failed to set speed to x%1. Error type: %2.").arg(double(m_speed)).arg(result));

    emit message(Logger::Type::Info, "FMOD", tr("Set speed to %1.").arg(double(m_speed)));
    emit speedChanged(m_speed, sender);
}

void MusicPlayer::reset()
{
    setPaused(true);
    QApplication::processEvents();
    emit positionChanged(0, this);

    if(m_music != nullptr)
        m_music->release();
    m_music = nullptr;

    m_channel = nullptr;

    emit message(Logger::Type::Info, "FMOD", tr("Music released."));
}

float MusicPlayer::speed() const
{
    return m_speed;
}

void MusicPlayer::setMusicFile(const QString &path)
{
    FMOD_RESULT result;

    setPaused(true);

    m_channel = nullptr;

    if(m_music != nullptr)
        m_music->release();
    m_music = nullptr;

    m_position = 0;
    emit positionChanged(0, this);

    result = m_fmodSystem->createSound(path.toLocal8Bit().data(), FMOD_DEFAULT, nullptr, &m_music);
    if(result != FMOD_OK)
    {
        emit message(Logger::Type::Error, "FMOD", tr("Failed to load music file \"%1\" on create FMOD::Sound. Error type: %2.").arg(path).arg(result));
        m_music = nullptr;
        return;
    }

    result = m_music->getLength(&m_length, FMOD_TIMEUNIT_MS);
    if(result != FMOD_OK)
    {
        m_music->release();
        m_music = nullptr;
        emit message(Logger::Type::Error, "FMOD", tr("Failed to get music Length. Error type: %1.").arg(result));
        return;
    }
    m_length--;

    result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
    if(result != FMOD_OK)
    {
        m_music->release();
        m_music = nullptr;
        emit message(Logger::Type::Error, "FMOD", tr("Failed to load music file \"%1\" on create FMOD::Channel. Error type: %2.").arg(path).arg(result));
        return;
    }

    result = m_channel->setPitch(m_speed);
    if(result != FMOD_OK)
        emit message(Logger::Type::Warning, "FMOD", tr("Failed to set music speed to x%1. Error type: %2.").arg(QString::number(double(m_speed), 'g', 2)).arg(result));

    emit message(Logger::Type::Info, "FMOD", tr("Loaded music \"%1\".").arg(path));
}

void MusicPlayer::forward(unsigned int frames)
{
    if(m_position / 20 == m_length / 20)
        setPosition(m_length);
    else
        setPosition((m_position / 20 + frames) * 20);
}

void MusicPlayer::back(unsigned int frames)
{
    if(m_position / 20 == 0)
        setPosition(0);
    else
        setPosition((m_position / 20 - frames) * 20);
}

unsigned int MusicPlayer::length() const
{
    return m_length;
}

unsigned int MusicPlayer::position() const
{
    return m_position;
}

void MusicPlayer::setMuted(bool muted, const QObject *sender)
{
    FMOD_RESULT result;

    m_muted = muted;

    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    if(m_channel == nullptr)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Music %1.").arg(m_muted ? tr("muted") : tr("unmuted")));
        emit this->muted(m_muted, sender);
        return;
    }

    bool isPlaying = false;
    result = m_channel->isPlaying(&isPlaying);
    if(result != FMOD_OK || !isPlaying)
    {
        emit message(Logger::Type::Info, "FMOD", tr("Music %1.").arg(m_muted ? tr("muted") : tr("unmuted")));
        emit this->muted(m_muted, sender);
        return;
    }

    result = m_channel->setMute(m_muted);
    if(result != FMOD_OK)
        emit message(Logger::Type::Warning, "FMOD", tr("Failed to %1 music. Error type: %2.").arg(m_muted ? tr("mute") : tr("unmute")).arg(result));

    emit message(Logger::Type::Info, "FMOD", tr("Music %1.").arg(m_muted ? tr("muted") : tr("unmuted")));
    emit this->muted(m_muted, sender);
}
