#include "metabarrage.h"

unsigned int MetaBarrage::m_barrageNumber = 0;

float MetaBarrage::m_globalPlaySpeed = 0.0f;

void MetaBarrage::reset()
{
	m_barrageNumber = 0;
	m_globalPlaySpeed = 1.0f;
};

void MetaBarrage::setPlaySpeed(float speed)
{
	m_globalPlaySpeed = speed;
};

unsigned int MetaBarrage::getBarrageNumber()
{
	return m_barrageNumber;
;}
