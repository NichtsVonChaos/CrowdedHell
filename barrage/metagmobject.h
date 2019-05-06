#ifndef METAGMOBJECT_H
#define METAGMOBJECT_H

#include <QMainWindow>
#include <QString>
#include <QTime>
#include <cmath>

class ResourceManager;
#ifndef RESOURCEMANAGER_H
#include "manager/resourcemanager.h"
#endif

#define PI 3.14159265358979
#define R2D(x) x / PI * 180
#define D2R(x) x / 180.0 * PI

/**
 * @brief The MetaBarrage class
 * Meta object of all kinds of barrages.
 * It is made as a state machine.
 */
class MetaGMObject : public QObject
{
	Q_OBJECT

public:
	MetaGMObject(ResourceManager *resourceManager);
	virtual ~MetaGMObject();
	virtual int getInteger(int attribute);
	virtual double getDouble(int attribute);
	virtual bool getBoolean(int attribute);
	virtual QString getString(int attribute) = 0;
	virtual void *getPointer(int attribute);

	virtual void setInteger(int attribute, int value);
	virtual void setDouble(int attribute, double value);
	virtual void setBoolean(int attribute, bool value);
	virtual void setString(int attribute, QString value) = 0;
	virtual void setPointer(int attribute, void *value) = 0;

	virtual void GMEvent_BeginStep() = 0;
	virtual void GMEvent_Alarm(unsigned int number) = 0;
	virtual void GMEvent_Step() = 0;
	virtual void GMEvent_Collision(MetaGMObject *object) = 0;
	virtual void GMEvent_EndStep() = 0;
	virtual void GMEvent_Draw(QWidget* widget) = 0;

	static unsigned int getInstanceNumber();

public slots:
	virtual void infer(QTime time) = 0;
	virtual void prevFrame() = 0;
	virtual void nextFrame() = 0;
	virtual void prev(unsigned int frames) = 0;
	virtual void next(unsigned int frames) = 0;

	static void reset();
	static void setPlaySpeed(float speed);

public:
	// Variables of integer number.
	static const int SPRITE_INDEX = 0;
	static const int IMAGE_INDEX = 1;
	static const int IMAGE_BLEND = 2;
	static const int DEPTH = 3;
	static const int PATH_INDEX = 4;
	static const int PATH_ENDACTION = 5;
	static const int ALARM_0 = 6;
	static const int ALARM_1 = 7;
	static const int ALARM_2 = 8;
	static const int ALARM_3 = 9;
	static const int ALARM_4 = 10;
	static const int ALARM_5 = 11;
	static const int ALARM_6 = 12;
	static const int ALARM_7 = 13;
	static const int ALARM_8 = 14;
	static const int ALARM_9 = 15;
	static const int ALARM_10 = 16;
	static const int ALARM_11 = 17;

	// Variables of double float point number.
	static const int SPEED = 0;
	static const int DIRECTION = 1;
	static const int HSPEED = 2;
	static const int VSPEED = 3;
	static const int FRICTION = 4;
	static const int GRAVITY = 5;
	static const int GRAVITY_DIRECTION = 6;
	static const int ALPHA = 7;
	static const int IMAGE_XSCALE = 8;
	static const int IMAGE_YSCALE = 9;
	static const int IMAGE_SPEED = 10;
	static const int IMAGE_ANGLE = 11;
	static const int PATH_POSITION = 12;
	static const int PATH_POSITIONPREVIOUS = 13;
	static const int PATH_ORIENTATION = 14;
	static const int PATH_SCALE = 15;
	static const int X = 16;
	static const int Y = 17;
	static const int XPREVIOUS = 18;
	static const int YPREVIOUS = 19;
	static const int XSTART = 20;
	static const int YSTART = 21;

	// Variables of boolean number.
	static const int VISIBLE = 0;
	static const int SOLID = 1;

	// Variables of pointer.
	static const int SELF = 0;

protected:
	static unsigned int m_instanceNumber;
	static float m_globalPlaySpeed;

	int m_sprite_index, m_image_index, m_image_blend, m_depth, m_path_index, m_path_endaction,
	m_alarm[12];
	double m_speed, m_direction, m_hspeed, m_vspeed, m_friction, m_gravity, m_gravity_direction,
	m_alpha, m_image_xscale, m_image_yscale, m_image_speed, m_image_angle,
	m_path_position, m_path_positionprevious, m_path_orientation, m_path_scale,
	m_x, m_y, m_xprevious, m_yprevious, m_xstart, m_ystart;
	bool m_visible, m_solid;

	ResourceManager *m_resourceManager;
};

#endif // METABARRAGE_H
