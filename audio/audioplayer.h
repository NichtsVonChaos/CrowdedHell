#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMessageBox>
#include <QTime>

#include "audioplayerslider.h"

#include "FMOD/inc/fmod.hpp"

class CrowdedHellGUI;
enum class MessageType;
enum class Language;

#ifndef CROWDEDHELLGUI_H
#include "crowdedhellgui.h"
#endif

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    AudioPlayer(AudioPlayerSlider *slider, CrowdedHellGUI *parent);

    bool isValid();

    void timerEvent(QTimerEvent *ev);

    void reselectMusic(const QString &path);

signals:
    void sendMessage(MessageType type, QString module, QString message);

    void playedOrPaused(bool played);

    void positionChanged(unsigned int pos);

public slots:
    void playOrPause(bool play);

    void changePosition(unsigned int pos);

private:
    bool m_valid, m_fmodNotInit;

    unsigned int m_pos;

    int m_timer;

    float m_volume;

    CrowdedHellGUI *m_parent;

    AudioPlayerSlider *m_slider;

    FMOD::System *m_fmodSystem;

    FMOD::Sound *m_music;

    FMOD::Channel *m_channel;

    void __initializeFmodSystem();
};

#endif // AUDIOPLAYER_H
