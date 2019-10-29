#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QtMath>

#include "Codes/GlobalComponent/globalComponent.h"
#include "Codes/ResourceManager/Music/musicplayer.h"

class VolumeSlider : public QSlider
{
    Q_OBJECT

public:
    explicit VolumeSlider(QWidget *parent = nullptr);
    ~VolumeSlider() override;

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

signals:
    /**
     * @brief volumeChanged
     * Emit a signal when button position changed.
     */
    void volumeChanged(float volume, const QObject *sender);

public slots:
    /**
     * @brief setVolume
     * Set button position.
     */
    void setVolume(float volume, const QObject *sender);

private slots:
    /**
     * @brief __valueChanged
     * A transfer slot.
     */
    void __valueChanged(int value);

private:
    const QObject *m_sender;
    bool m_leftButtonPressing;
};

#endif // VOLUMESLIDER_H
