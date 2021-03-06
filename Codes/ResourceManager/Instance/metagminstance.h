#ifndef METAGMINSTANCE_H
#define METAGMINSTANCE_H

#include <QObject>

#include "../Image/sprite.h"

/**
 * @brief The MetaObject class
 * The MetaObject is a abstract base class to emulate GM8 instance.
 */
class MetaGMInstance : public QObject
{
    Q_OBJECT
public:
    explicit MetaGMInstance(QObject *parent = nullptr);

    MetaGMInstance(const MetaGMInstance&) = delete;
    MetaGMInstance& operator=(const MetaGMInstance&) = delete;
    MetaGMInstance(MetaGMInstance&&) = delete;
    MetaGMInstance& operator=(MetaGMInstance&&) = delete;

    virtual ~MetaGMInstance();

    virtual void GMEvent_BeginStep() = 0;
    virtual void GMEvent_Alarm(unsigned int number) = 0;
    virtual void GMEvent_Step() = 0;
    virtual void GMEvent_Collision(MetaGMInstance *object) = 0;
    virtual void GMEvent_EndStep() = 0;
    virtual void GMEvent_Draw(QWidget* widget) = 0;

    virtual void infer(unsigned int time) = 0;
    virtual void prevFrame() = 0;
    virtual void nextFrame() = 0;
    virtual void prev(unsigned int frames = 1) = 0;
    virtual void next(unsigned int frames = 1) = 0;

    virtual void toXml() = 0;

public:
    long long sprite_index, image_index, image_blend, path_index, path_endaction,
    alarm[12];
    double speed, direction, hspeed, vspeed, friction, gravity, gravity_direction,
    alpha, image_xscale, image_yscale, image_speed, image_angle,
    path_position, path_orientation, path_scale,
    x, y, xprevious, yprevious, xstart, ystart;
    bool visible, solid;

signals:

public slots:

};

#endif // METAGMINSTANCE_H
