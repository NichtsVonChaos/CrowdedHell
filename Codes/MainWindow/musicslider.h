#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QtMath>

#include "Codes/GlobalComponent/logger.h"
#include "Codes/GlobalComponent/optionsmanager.h"
#include "Codes/GlobalComponent/project.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

class MusicSlider : public QSlider
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
    ~MusicSlider() override;
    void initialze(MusicPlayer *musicPlayer);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void positionChanged(unsigned int pos, const QObject *sender);

public slots:
    void setPosition(unsigned int pos, const QObject *sender);

private slots:
    void __valueChanged(int value);

private:
    MusicPlayer *m_musicPlayer;
    const QObject *m_sender;
    bool m_leftButtonPressing;
};

#endif // MUSICSLIDER_H
