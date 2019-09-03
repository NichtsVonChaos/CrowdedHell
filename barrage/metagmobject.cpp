#include "metagmobject.h"

unsigned int MetaGMObject::m_instanceNumber = 0;

float MetaGMObject::m_globalPlaySpeed = 0.0f;

MetaGMObject::MetaGMObject(ResourceManager *resourceManager) :
	m_resourceManager(resourceManager)
{
	m_sprite_index = -1;
	m_image_index = 0;
	m_image_blend = 0xFFFFFF;
	m_depth = 0;
	m_path_index = -1;
	m_path_endaction = 0;
	for(int i = 0; i < 12; i++)
		m_alarm[i] = -1;

	m_speed = 0.0;
	m_direction = 0.0;
	m_hspeed = 0.0;
	m_vspeed = 0.0;
	m_friction = 0.0;
	m_gravity=0.0;
	m_gravity_direction = 0.0;
	m_alpha = 1.0;
	m_image_xscale = 1.0;
	m_image_yscale = 1.0;
	m_image_speed= 1.0;
	m_image_angle = 0.0;
	m_path_position = 0.0;
	m_path_positionprevious = 0.0;
	m_path_orientation = 0.0;
	m_path_scale = 1.0;
	m_x = 0.0;
	m_y = 0.0;
	m_xprevious = 0.0;
	m_yprevious = 0.0;
	m_xstart=0.0;
	m_ystart = 0.0;

	m_visible = true;
	m_solid = false;
}

MetaGMObject::~MetaGMObject()
{

};

int MetaGMObject::getInteger(int attribute)
{
	{
		switch(attribute)
		{
			case SPRITE_INDEX:
			return m_sprite_index;

			case IMAGE_INDEX:
			return m_image_index;

			case IMAGE_BLEND:
			return m_image_blend;

			case DEPTH:
			return m_depth;

			case PATH_INDEX:
			return m_path_index;

			case PATH_ENDACTION:
			return m_path_endaction;

			case ALARM_0:
			return m_alarm[0];

			case ALARM_1:
			return m_alarm[1];

			case ALARM_2:
			return m_alarm[2];

			case ALARM_3:
			return m_alarm[3];

			case ALARM_4:
			return m_alarm[4];

			case ALARM_5:
			return m_alarm[5];

			case ALARM_6:
			return m_alarm[6];

			case ALARM_7:
			return m_alarm[7];

			case ALARM_8:
			return m_alarm[8];

			case ALARM_9:
			return m_alarm[9];

			case ALARM_10:
			return m_alarm[10];

			case ALARM_11:
			return m_alarm[11];

			default:
			return 0;
		}
	}
};

double MetaGMObject::getDouble(int attribute)
{
	switch(attribute)
	{
		case SPEED:
		return m_speed;

		case DIRECTION:
		return m_direction;

		case HSPEED:
		return m_hspeed;

		case VSPEED:
		return m_vspeed;

		case FRICTION:
		return m_friction;

		case GRAVITY:
		return m_gravity;

		case GRAVITY_DIRECTION:
		return m_gravity_direction;

		case ALPHA:
		return m_alpha;

		case IMAGE_XSCALE:
		return m_image_xscale;

		case IMAGE_YSCALE:
		return m_image_yscale;

		case IMAGE_SPEED:
		return m_image_speed;

		case IMAGE_ANGLE:
		return m_image_angle;

		case PATH_POSITION:
		return m_path_position;

		case PATH_POSITIONPREVIOUS:
		return m_path_positionprevious;

		case PATH_ORIENTATION:
		return m_path_orientation;

		case PATH_SCALE:
		return m_path_scale;

		case X:
		return m_x;

		case Y:
		return m_y;

		case XPREVIOUS:
		return m_xprevious;

		case YPREVIOUS:
		return m_yprevious;

		case XSTART:
		return m_xstart;

		case YSTART:
		return m_ystart;

		default:
		return 0.0;
	}
};

bool MetaGMObject::getBoolean(int attribute)
{
	switch(attribute)
	{
		case VISIBLE:
		return m_visible;

		case SOLID:
		return m_solid;

		default:
		return false;
	}
};

void *MetaGMObject::getPointer(int attribute)
{
	switch(attribute)
	{
		case SELF:
		return static_cast<void*>(this);

		default:
		return nullptr;
	}
};

void MetaGMObject::setInteger(int attribute, int value)
{
	switch(attribute)
	{
		case SPRITE_INDEX:
			m_sprite_index = value;
		break;

		case IMAGE_INDEX:
			m_image_index = value;
		break;

		case IMAGE_BLEND:
			m_image_blend = value;
		break;

		case DEPTH:
			m_depth = value;
		break;

		case PATH_INDEX:
			m_path_index = value;
		break;

		case PATH_ENDACTION:
			m_path_endaction = value;
		break;

		case ALARM_0:
			m_alarm[0] = value;
		break;

		case ALARM_1:
			m_alarm[1] = value;
		break;

		case ALARM_2:
			m_alarm[2] = value;
		break;

		case ALARM_3:
			m_alarm[3] = value;
		break;

		case ALARM_4:
			m_alarm[4] = value;
		break;

		case ALARM_5:
			m_alarm[5] = value;
		break;

		case ALARM_6:
			m_alarm[6] = value;
		break;

		case ALARM_7:
			m_alarm[7] = value;
		break;

		case ALARM_8:
			m_alarm[8] = value;
		break;

		case ALARM_9:
			m_alarm[9] = value;
		break;

		case ALARM_10:
			m_alarm[10] = value;
		break;

		case ALARM_11:
			m_alarm[11] = value;
		break;

	}
};

void MetaGMObject::setDouble(int attribute, double value)
{
	switch(attribute)
	{
		case SPEED:
		{
			m_speed = value;
			m_hspeed = m_speed * cos(D2R(m_direction));
			m_vspeed = -(m_speed * sin(D2R(m_direction)));
		}
		break;

		case DIRECTION:
		{
			m_direction = value;
			m_hspeed = m_speed * cos(D2R(m_direction));
			m_vspeed = -(m_speed * sin(D2R(m_direction)));
		}
		break;

		case HSPEED:
		{
			m_hspeed = value;
			m_speed = sqrt(m_vspeed * m_vspeed + m_hspeed * m_hspeed);
			m_direction = R2D(atan2(-m_vspeed , m_hspeed));
		}
		break;

		case VSPEED:
		{
			m_vspeed = value;
			m_speed = sqrt(m_vspeed * m_vspeed + m_hspeed * m_hspeed);
			m_direction = R2D(atan2(-m_vspeed , m_hspeed));
		}
		break;

		case FRICTION:
			m_friction = value;
		break;

		case GRAVITY:
			m_gravity = value;
		break;

		case GRAVITY_DIRECTION:
			m_gravity_direction = value;
		break;

		case ALPHA:
			m_alpha = value;
		break;

		case IMAGE_XSCALE:
			m_image_xscale = value;
		break;

		case IMAGE_YSCALE:
			m_image_yscale = value;
		break;

		case IMAGE_SPEED:
			m_image_speed = value;
		break;

		case IMAGE_ANGLE:
			m_image_angle = value;
		break;

		case PATH_POSITION:
			m_path_position = value;
		break;

		case PATH_POSITIONPREVIOUS:
			m_path_positionprevious = value;
		break;

		case PATH_ORIENTATION:
			m_path_orientation = value;
		break;

		case PATH_SCALE:
			m_path_scale = value;
		break;

		case X:
			m_x =value;
		break;

		case Y:
			m_y = value;
		break;

		case XPREVIOUS:
			m_xprevious = value;
		break;

		case YPREVIOUS:
			m_yprevious = value;
		break;

		case XSTART:
			m_xstart = value;
		break;

		case YSTART:
			m_ystart = value;
		break;
	}
};

void MetaGMObject::setBoolean(int attribute, bool value)
{
	switch(attribute)
	{
		case VISIBLE:
			m_visible = value;
		break;

		case SOLID:
			m_solid = value;
		break;
	}
};

void MetaGMObject::reset()
{
	m_instanceNumber = 0;
	m_globalPlaySpeed = 1.0f;
};

void MetaGMObject::setPlaySpeed(float speed)
{
	m_globalPlaySpeed = speed;
};

unsigned int MetaGMObject::getInstanceNumber()
{
	return m_instanceNumber;
};
