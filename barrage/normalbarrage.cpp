#include "normalbarrage.h"

NormalBarrage::NormalBarrage(ResourceManager *resourceManager, int level) :
	MetaGMObject(resourceManager), m_level(level)
{
	m_instanceNumber++;
};;

NormalBarrage::~NormalBarrage()
{
	m_instanceNumber--;
};

int NormalBarrage::getInteger(int attribute)
{
	if(attribute <= ALARM_11)
		return MetaGMObject::getInteger(attribute);
	else switch(attribute)
	{
		default:
		return 0;
	}
};

double NormalBarrage::getDouble(int attribute)
{
	if(attribute <= YSTART)
		return MetaGMObject::getDouble(attribute);
	else switch(attribute)
	{
		default:
		return 0;
	}
};

bool NormalBarrage::getBoolean(int attribute)
{
	if(attribute <= SOLID)
		return MetaGMObject::getBoolean(attribute);
	else switch(attribute)
	{
		default:
		return false;
	}
};

QString NormalBarrage::getString(int attribute)
{
	switch(attribute)
	{
		default:
		return QString();
	}
};

void *NormalBarrage::getPointer(int attribute)
{
	if(attribute <= SELF)
		return MetaGMObject::getPointer(attribute);
	else switch(attribute)
	{
		default:
		return nullptr;
	}
};

void NormalBarrage::setInteger(int attribute, int value)
{
	if(attribute <= ALARM_11)
		MetaGMObject::setInteger(attribute, value);
	else switch(attribute)
	{

	}
};

void NormalBarrage::setDouble(int attribute, double value)
{
	if(attribute <= YSTART)
		MetaGMObject::setDouble(attribute, value);
	else switch(attribute)
	{

	}
};

void NormalBarrage::setBoolean(int attribute, bool value)
{
	if(attribute <= SOLID)
		MetaGMObject::setBoolean(attribute, value);
	else switch(attribute)
	{

	}
};

void NormalBarrage::setString(int attribute, QString value)
{

};

void NormalBarrage::setPointer(int attribute, void *value)
{

};

void NormalBarrage::GMEvent_BeginStep()
{

};

void NormalBarrage::GMEvent_Alarm(unsigned int number)
{

};

void NormalBarrage::GMEvent_Step()
{

};

void NormalBarrage::GMEvent_Collision(MetaGMObject *object)
{

};

void NormalBarrage::GMEvent_EndStep()
{

};

void NormalBarrage::GMEvent_Draw(QWidget *widget)
{

};

void NormalBarrage::infer(QTime time)
{

};

void NormalBarrage::prevFrame()
{

};

void NormalBarrage::nextFrame()
{

};

void NormalBarrage::prev(unsigned int frames)
{

};

void NormalBarrage::next(unsigned int frames)
{

};
