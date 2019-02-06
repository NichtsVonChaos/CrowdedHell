#include "audioplayer.h"

AudioPlayer::AudioPlayer(AudioPlayerSlider *slider, CrowdedHellGUI *parent) :
	m_fmodNotInit(true), m_muted(false), m_pos(0), m_length(0), m_speed(1.0f), m_volume(0.6f), m_parent(parent), m_slider(slider), m_fmodSystem(nullptr), m_music(nullptr), m_channel(nullptr)
{
	connect(this, SIGNAL(sendMessage(MessageType, QString, QString)),
			parent, SLOT(sendMessage(MessageType, QString, QString)));

	connect(this, SIGNAL(playedOrPaused(bool)), parent, SLOT(musicPlayedOrPaused(bool)));

	connect(this, SIGNAL(positionChanged(unsigned int)), parent, SLOT(musicPositionChanged(unsigned int)));

	__readSettings();

	m_slider->setAudioPlayer(this);

	__initializeFmodSystem();

	m_timer = QObject::startTimer(5);
};

AudioPlayer::~AudioPlayer()
{
	playOrPause(false);

	if(m_music != nullptr)
		m_music->release();

	if(m_channel != nullptr)
		delete m_channel;

	if(m_fmodSystem != nullptr)
		m_fmodSystem->release();
};

void AudioPlayer::timerEvent(QTimerEvent *ev)
{
	if(ev->timerId() == m_timer)
	{
		if(m_channel == nullptr)
		{
			m_pos = 0;
			emit positionChanged(0);
			return;
		}

		unsigned int last_pos = m_pos;
		FMOD_RESULT result;

		result = m_channel->getPosition(&m_pos, FMOD_TIMEUNIT_MS);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to get current position."));
			playOrPause(true);
			return;
		}

		if(last_pos != m_pos)
		{
			if(m_pos >= m_length - 5 * m_speed)
			{
				emit sendMessage(MessageType::Info, "FMOD", tr("Music is at the end."));
				m_pos = m_length;
				m_channel->setPaused(true);
				result = m_channel->setPosition(m_pos, FMOD_TIMEUNIT_MS);
				if(result != FMOD_OK)
				{
					emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set position to %1 miliseconds while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
					playOrPause(true);
					return;
				}
				emit playedOrPaused(false);
			}
			emit positionChanged(m_pos);
			m_slider->setValue(int(double(m_pos) * 1000 / double(m_length)));
		}
	}
};

void AudioPlayer::changeMusic(const QString &path)
{
	playOrPause(false);

	if(m_music != nullptr)
		m_music->release();
	m_music = nullptr;

	if(m_channel != nullptr)
		delete m_channel;
	m_channel = nullptr;

	m_parent->updateMusicLength(0);
	m_pos = 0;
	emit positionChanged(0);
	m_slider->setValue(0);

	FMOD_RESULT result;

	result = m_fmodSystem->createSound(path.toLocal8Bit().data(), FMOD_2D, nullptr, &m_music);
	if(result != FMOD_OK)
	{
		emit sendMessage(MessageType::Error, "FMOD", tr("Failed to load sound file %1 .").arg(QString("\"") + path + QString("\".")));
		m_music = nullptr;
		return;
	}
	else
		emit sendMessage(MessageType::Info, "FMOD", tr("Loaded sound file %1 .").arg( QString(" \"") + path + QString("\".")));

	result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);

	if(result != FMOD_OK)
	{
		emit sendMessage(MessageType::Error, "FMOD", tr("Failed to create channel."));
		emit playedOrPaused(false);
		return;
	}
	else
		emit sendMessage(MessageType::Info, "FMOD", tr("Create channel successfully."));

	result = m_channel->setPitch(m_speed);
	if(result != FMOD_OK)
	{
		emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set music speed to x%1.").arg(QString::number(double(m_speed), 'g', 2)));
		emit playedOrPaused(false);
		return;
	}
	else
		emit sendMessage(MessageType::Info, "FMOD", tr("Set music speed to x%1.").arg(QString::number(double(m_speed), 'g', 2)));

	m_music->getLength(&m_length, FMOD_TIMEUNIT_MS);
	m_length -= 5;
	m_parent->updateMusicLength(m_length);
};

void AudioPlayer::forward(unsigned int frames)
{
	if(m_pos / 20 == m_length / 20)
		changePosition(m_length);
	else
		changePosition((m_pos / 20 + frames) * 20);
};

void AudioPlayer::back(unsigned int frames)
{
	if(m_pos / 20 == 0)
		changePosition(0);
	else
		changePosition((m_pos / 20 - frames) * 20);
};

unsigned int AudioPlayer::getPosition()
{
	if(m_channel == nullptr)
		return 0;
	else
	{
		unsigned int pos;
		m_channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
		return pos;
	}
};

unsigned int AudioPlayer::getMusicLength()
{
	return m_length;
};

float AudioPlayer::getSpeed()
{
	return m_speed;
};

float AudioPlayer::getVolume()
{
	return m_volume;
};

bool AudioPlayer::isPlaying()
{
	if(m_music == nullptr)
		return false;
	else
	{
		bool isPlaying;
		m_channel->isPlaying(&isPlaying);
		return isPlaying;
	}
};

void AudioPlayer::reset()
{
	playOrPause(false);

	if(m_music != nullptr)
		m_music->release();
	m_music = nullptr;

	if(m_channel != nullptr)
		delete m_channel;
	m_channel = nullptr;

	m_parent->updateMusicLength(0);
	m_pos = 0;
	emit positionChanged(0);
	m_slider->setValue(0);
};

void AudioPlayer::playOrPause(bool play)
{
	FMOD_RESULT result;

	if(play)
	{
		if(m_music == nullptr)
		{
			emit sendMessage(MessageType::Warning, "FMOD", tr("No Background music was selected or selected music is invalid."));
			emit playedOrPaused(false);
			m_parent->musicInvalid();
			return;
		}

		__initializeFmodSystem();

		if(m_channel == nullptr)
		{
			emit sendMessage(MessageType::Info, "FMOD", tr("No channel was found. Trying to create channel..."));
			result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
			if(result != FMOD_OK)
			{
				emit sendMessage(MessageType::Error, "FMOD", tr("Failed to create channel."));
				emit playedOrPaused(false);
				return;
			}
			else
				emit sendMessage(MessageType::Info, "FMOD", tr("Create channel successfully."));
		}

		bool isPaused;
		m_channel->getPaused(&isPaused);
		if(!isPaused)
			return;

		float volume;
		result = m_channel->getVolume(&volume);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to get current volume."));
			emit playedOrPaused(false);
			return;
		}

		if(abs(volume - m_volume) > 0.0001f)
		{
			result = m_channel->setVolume(m_volume);
			if(result != FMOD_OK)
			{
				emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set volume to %1 percent.").arg(QString::number(double(m_volume * 100))));
				emit playedOrPaused(false);
				return;
			}
			else
				emit sendMessage(MessageType::Info, "FMOD", tr("Set volume to %1 percent successful.").arg(QString::number(double(m_volume * 100))));
		}

		bool mute;
		result = m_channel->getMute(&mute);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to get mute."));
			emit playedOrPaused(false);
			return;
		}

		if(mute != m_muted)
		{
			result = m_channel->setMute(m_muted);
			if(m_muted)
			{
				if(result != FMOD_OK)
					sendMessage(MessageType::Error, "FMOD", tr("Failed to mute music."));
				else
					emit sendMessage(MessageType::Info, "FMOD", tr("Mute music."));
			}
			else
			{
				if(result != FMOD_OK)
					sendMessage(MessageType::Error, "FMOD", tr("Failed to unmute music."));
				else
					emit sendMessage(MessageType::Info, "FMOD", tr("Unmute music."));
			}
		}

		if(m_pos >= m_length)
		{
			emit sendMessage(MessageType::Warning, "FMOD", tr("Music is at the end."));
			m_pos = m_length;
			emit playedOrPaused(false);
			emit positionChanged(m_length);
			return;
		}

		unsigned int currentPosition = 0;
		result = m_channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to get current position."));
			return;
		}

		if(currentPosition != m_pos)
		{
			result = m_channel->setPosition(unsigned(m_pos), FMOD_TIMEUNIT_MS);
			if(result != FMOD_OK)
			{
				emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set position to %1 miliseconds while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
				m_pos = m_length;
				emit positionChanged(m_length);
				emit playedOrPaused(false);
				return;
			}
			else
				emit sendMessage(MessageType::Info, "FMOD", tr("Set position to %1 miliseconds successfully while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
		}

		result = m_channel->setPaused(false);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to play music."));
			m_pos = 0;
			emit playedOrPaused(false);
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD",  m_pos == 0 ? tr("Music Started.") : tr("Music Resumed."));

		emit playedOrPaused(true);
	}
	else
	{
		if(m_music == nullptr)
			return;

		if(m_channel == nullptr)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("If you have met this message, it means that my codes have logic error. Please report to me detailedly, thanks!"));
			emit playedOrPaused(false);
			return;
		}

		bool isPaused;
		m_channel->getPaused(&isPaused);
		if(isPaused)
			return;

		result = m_channel->setPaused(true);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to pause music."));
			m_pos = 0;
			emit playedOrPaused(false);
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD",tr("Music paused."));

		emit playedOrPaused(false);
	}
};

void AudioPlayer::changePosition(unsigned int pos)
{
	FMOD_RESULT result;

	if(m_music == nullptr)
	{
		emit sendMessage(MessageType::Warning, "FMOD", tr("No Background music was selected or selected music is invalid."));
		emit playedOrPaused(false);
		m_parent->musicInvalid();
		return;
	}

	__initializeFmodSystem();

	if(m_channel == nullptr)
	{
		emit sendMessage(MessageType::Info, "FMOD", tr("No channel was found. Trying to create channel..."));
		result = m_fmodSystem->playSound(m_music, nullptr, true, &m_channel);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to create channel."));
			emit playedOrPaused(false);
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("Create channel successfully."));
	}

	if(pos >= m_length)
	{
		emit sendMessage(MessageType::Warning, "FMOD", tr("Position %1 miliseconds is equal with or larger than music length %2 miliseconds, therefore set the position to the end of music.").arg(QString::number(pos), QString::number(m_length)));
		m_pos = m_length;
		result = m_channel->setPosition(m_length, FMOD_TIMEUNIT_MS);
		if(result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set position to %1 miliseconds while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
			m_pos = 0;
			m_channel->setPaused(true);
			emit playedOrPaused(false);
			return;
		}
		m_channel->setPaused(true);
		emit playedOrPaused(false);
		emit positionChanged(m_length);
		return;
	}

	unsigned int currentPosition = 0;
	result = m_channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
	if(result != FMOD_OK)
	{
		emit sendMessage(MessageType::Error, "FMOD", tr("Failed to get current position."));
		return;
	}

	if(currentPosition == pos)
		return;

	m_pos = pos;
	result = m_channel->setPosition(m_pos, FMOD_TIMEUNIT_MS);
	if(result != FMOD_OK)
	{
		emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set position to %1 miliseconds while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
		m_pos = 0;
		m_channel->setPaused(true);
		emit playedOrPaused(false);
		return;
	}

	m_slider->setValue(int(double(m_pos) * 1000 / double(m_length)));
	emit sendMessage(MessageType::Info, "FMOD", tr("Set position to %1 miliseconds successfully while the length of music is %2 miliseconds.").arg(QString::number(m_pos), QString::number(m_length)));
	emit positionChanged(pos);
};

void AudioPlayer::changeSpeed(float speed)
{
	m_speed = speed;
	if(m_channel != nullptr)
	{
		FMOD_RESULT result = m_channel->setPitch(m_speed);
		if(result != FMOD_OK)
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set music speed to x%1.").arg(QString::number(double(m_speed), 'g', 2)));
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("Set music speed to x%1.").arg(QString::number(double(m_speed), 'g', 2)));
	}

	emit speedChanged(speed);
};

void AudioPlayer::changeVolume(float volume)
{
	m_volume = volume;
	if(m_channel != nullptr)
	{
		FMOD_RESULT result = m_channel->setVolume(volume);
		if(result != FMOD_OK)
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to set music volume to %1/100.").arg(QString::number(int(m_volume * 100))));
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("Set music volume to %1/100.").arg(QString::number(int(m_volume * 100))));
	}

	__updateSettings();
};

void AudioPlayer::mute(bool mute)
{
	if(m_muted != mute)
	{
		m_muted = mute;
		if(m_channel != nullptr)
		{
			FMOD_RESULT result = m_channel->setMute(m_muted);

			if(m_muted)
			{
				if(result != FMOD_OK)
					emit sendMessage(MessageType::Error, "FMOD", tr("Failed to mute music."));
				else
					emit sendMessage(MessageType::Info, "FMOD", tr("Mute music."));
			}
			else
			{
				if(result != FMOD_OK)
					emit sendMessage(MessageType::Error, "FMOD", tr("Failed to unmute music."));
				else
					emit sendMessage(MessageType::Info, "FMOD", tr("Unmute music."));
			}
		}
	}
};

void AudioPlayer::__initializeFmodSystem()
{
	FMOD_RESULT result;

	if(m_fmodSystem == nullptr)
	{
		result = FMOD::System_Create(&m_fmodSystem);

		if (result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to create FMOD system."));
			m_fmodSystem = nullptr;
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("Created FMOD system successfully."));

		int driverCount = 0;
		m_fmodSystem->getNumDrivers(&driverCount);

		if (driverCount == 0)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to load sound drivers."));
			delete m_fmodSystem;
			m_fmodSystem = nullptr;
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("%1 sound driver(s) was found.").arg(driverCount));
	}

	if(m_fmodNotInit)
	{
		result = m_fmodSystem->init(36, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			emit sendMessage(MessageType::Error, "FMOD", tr("Failed to initialize FMOD system."));
			m_fmodNotInit = true;
			return;
		}
		else
			emit sendMessage(MessageType::Info, "FMOD", tr("Initialize FMOD system successfully."));

		m_fmodNotInit = false;
	}
};

void AudioPlayer::__readSettings()
{
	QSettings iniFile("./settings.ini", QSettings::IniFormat);
	iniFile.beginGroup("Audio");
	m_volume = iniFile.value("Volume", 0.6f).toFloat();
	iniFile.endGroup();
};

void AudioPlayer::__updateSettings()
{
	QSettings iniFile("./settings.ini", QSettings::IniFormat);
	iniFile.beginGroup("Audio");
	iniFile.setValue("Volume", m_volume);
	iniFile.endGroup();
};
