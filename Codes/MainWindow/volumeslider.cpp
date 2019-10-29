#include "volumeslider.h"

VolumeSlider::VolumeSlider(QWidget *parent):
    QSlider(parent)
{
    setMaximum(100);
    connect(this, &VolumeSlider::volumeChanged, options(), &Options::setVolume, Qt::UniqueConnection);
    connect(this, &VolumeSlider::valueChanged, this, &VolumeSlider::__valueChanged, Qt::UniqueConnection);
}

VolumeSlider::~VolumeSlider()
{

}

void VolumeSlider::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        disconnect(this, &VolumeSlider::volumeChanged, options(), &Options::setVolume);
        disconnect(options(), &Options::volumeChanged, this, &VolumeSlider::setVolume);
        int pos = limit(int(100.0 * (double(ev->x()) / width())), 0, 100);
        setValue(pos);
        m_leftButtonPressing = true;
        ev->accept();
    }
}

void VolumeSlider::mouseMoveEvent(QMouseEvent *ev)
{
    if(m_leftButtonPressing)
    {
        int pos = limit(int(100.0 * (double(ev->x()) / width())), 0, 100);
        setValue(pos);
        ev->accept();
    }
}

void VolumeSlider::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        int pos = limit(int(100.0 * (double(ev->x()) / width())), 0, 100);
        connect(this, &VolumeSlider::volumeChanged, options(), &Options::setVolume, Qt::UniqueConnection);
        connect(options(), &Options::volumeChanged, this, &VolumeSlider::setVolume, Qt::UniqueConnection);
        setValue(pos);
        __valueChanged(pos);
        m_leftButtonPressing = false;
        ev->accept();
    }
}

void VolumeSlider::setVolume(float volume, const QObject *sender)
{
    if(sender == this)
        return;

    if(sender == nullptr)
        sender = this;

    m_sender = sender;
    volume = limit(volume, 0.f, 1.f);
    setValue(int(volume * 100));
}

void VolumeSlider::__valueChanged(int value)
{
    float volume = limit(float(value) / 100.f, 0.f, 1.f);
    emit volumeChanged(volume, m_sender);
    m_sender = this;
}
