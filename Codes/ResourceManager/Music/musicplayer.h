#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QTime>
#include <FMOD/inc/fmod.hpp>

#include "Codes/GlobalComponent/logger.h"
#include "Codes/GlobalComponent/optionsmanager.h"

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer() override;


    void timerEvent(QTimerEvent *ev) override;

signals:
    void message(const QString &module, Logger::Type type, const QString &message);

public slots:
    void setVolume(float volume, QObject *sender = nullptr);
};

#endif // MUSICPLAYER_H
