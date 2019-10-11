#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QSlider>

#include "Codes/GlobalComponent/logger.h"
#include "Codes/GlobalComponent/optionsmanager.h"
#include "Codes/GlobalComponent/project.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

class MusicSlider : public QSlider
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
    ~MusicSlider();
    void initialze(MusicPlayer *musicPlayer);

signals:
    void positionChanged(unsigned int pos, QObject *sender);

public slots:
    void setPosition(unsigned int pos, QObject *sender);

private slots:
    void __valueChanged(int value);

private:
    MusicPlayer *m_musicPlayer;
    QObject *m_sender;
};

#endif // MUSICSLIDER_H
