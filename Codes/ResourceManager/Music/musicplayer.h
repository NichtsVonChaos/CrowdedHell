#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QTime>
#include <QTimerEvent>
#include <FMOD/inc/fmod.hpp>

#include "Codes/GlobalComponent/globalComponent.h"

/**
 * @brief The MusicPlayer class
 * The music player is a encapsulation of FMOD,
 * so that you can avoid to contact FMOD on other modules directly.
 */
class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer() override;

    /**
     * @brief position
     * Get current position (milliseconds).
     */
    unsigned int position() const;

    /**
     * @brief length
     * Get music length (milliseconds).
     */
    unsigned int length() const;

    /**
     * @brief speed
     * Get music speed.
     * Default is 1.0.
     */
    float speed() const;

    /**
     * @brief setMusicFile
     * Set music file to play.
     */
    void setMusicFile(const QString &path);

    /**
     * @brief forward
     * Set music position to (cur_pos / 20 + frames) * 20.
     */
    void forward(unsigned int frames);

    /**
     * @brief back
     * Set music position to (cur_pos / 20 - frames) * 20.
     */
    void back(unsigned int frames);

    void timerEvent(QTimerEvent *ev) override;

signals:
    /**
     * @see Logger::message.
     */
    void message(Logger::Type type, const QString &module, const QString &message);

    /**
     * @brief paused
     * Emit a signal when music pause or resume.
     * @param paused
     * If pause, true.
     * If resume, false.
     */
    void paused(bool paused, const QObject *sender);

    /**
     * @brief muted
     * Emit a signal when music mute or unmute.
     * @param paused
     * If mute, true.
     * If unmute, false.
     */
    void muted(bool muted, const QObject *sender);

    /**
     * @brief positionChanged
     * Emit a signal when music position changed.
     * @param pos
     * Current position (milliseconds).
     */
    void positionChanged(unsigned int pos, const QObject *sender);

    /**
     * @brief speedChanged
     * Emit a signal when music speed changed.
     * @param speed
     * Current speed.
     */
    void speedChanged(float speed, const QObject *sender);

public slots:
    /**
     * @brief setVolume
     * Set volume.
     * Note this function should not be called directly.
     * Please call options()->setVolume() to change volume.
     * @param volume
     * @param sender
     */
    void setVolume(float volume, const QObject *sender = nullptr);

    /**
     * @brief pause
     * Pause music. Equal with setPaused(false).
     * Normally, you'd better call setPaused instead.
     * But sometimes you may need it for connect signal and slots.
     */
    void pause();

    /**
     * @brief setPaused
     * Pause or resume music.
     * @param paused
     * If true, pause music.
     * If false, resume music.
     */
    void setPaused(bool paused, const QObject *sender = nullptr);

    /**
     * @brief setMuted
     * Mute or unmute music.
     * @param muted
     * If true, mute music.
     * If false, unmute music.
     */
    void setMuted(bool muted, const QObject *sender = nullptr);

    /**
     * @brief setPosition
     * Set music position.
     * @param pos
     * Music position (milliseconds).
     */
    void setPosition(unsigned int pos, const QObject *sender = nullptr);

    /**
     * @brief setSpeed
     * Set music speed.
     * @param speed
     * Music speed (normally 1.0).
     */
    void setSpeed(float speed, const QObject *sender = nullptr);

    /**
     * @brief reset
     * Reset and back to initial status.
     */
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
