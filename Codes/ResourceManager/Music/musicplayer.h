#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QTime>
#include <QTimerEvent>
#include <FMOD/inc/fmod.hpp>

#include "Codes/GlobalComponent/logger.h"
#include "Codes/GlobalComponent/optionsmanager.h"

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer() override;

    unsigned int position() const;
    unsigned int length() const;
    float speed() const;

    void setMusicFile(const QString &path);
    void forward(unsigned int frames);
    void back(unsigned int frames);

    void timerEvent(QTimerEvent *ev) override;

signals:
    void message(Logger::Type type, const QString &module, const QString &message);
    void paused(bool paused, const QObject *sender);
    void muted(bool muted, const QObject *sender);
    void positionChanged(unsigned int pos, const QObject *sender);
    void speedChanged(float speed, const QObject *sender);

public slots:
    void setVolume(float volume, const QObject *sender = nullptr);
    void pause();
    void setPaused(bool paused, const QObject *sender = nullptr);
    void setMuted(bool muted, const QObject *sender = nullptr);
    void setPosition(unsigned int pos, const QObject *sender = nullptr);
    void setSpeed(float speed, const QObject *sender = nullptr);
    void reset();

private:
    FMOD::System *m_fmodSystem;
    FMOD::Sound *m_music;
    FMOD::Channel *m_channel;

    bool m_muted;
    unsigned int m_position, m_length;
    float m_speed;

    int m_timer;

    bool m_timerReportError;
};

#endif // MUSICPLAYER_H
