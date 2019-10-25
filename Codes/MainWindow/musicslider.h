#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QtMath>

#include "Codes/GlobalComponent/globalComponent.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

/**
 * @brief The MusicSlider class
 */
class MusicSlider : public QSlider
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
    ~MusicSlider() override;

    /**
     * @brief initialze
     * Connect signals and slots between self and music player.
     */
    void initialze(MusicPlayer *musicPlayer);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    /**
     * @brief positionChanged
     * Emit a signal when button position changed.
     */
    void positionChanged(unsigned int pos, const QObject *sender);

public slots:
    /**
     * @brief setPosition
     * Set button position.
     */
    void setPosition(unsigned int pos, const QObject *sender);

private slots:
    /**
     * @brief __valueChanged
     * A transfer slot.
     */
    void __valueChanged(int value);

private:
    MusicPlayer *m_musicPlayer;
    const QObject *m_sender;
    bool m_leftButtonPressing;
};

#endif // MUSICSLIDER_H
