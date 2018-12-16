#include "metabarrage.h"

int MetaBarrage::m_barrageNumber = 0;

double MetaBarrage::m_globalPlaySpeed = 0.0;

void MetaBarrage::restart()
{
	m_barrageNumber = 0;
	m_globalPlaySpeed = 1.0;
};
