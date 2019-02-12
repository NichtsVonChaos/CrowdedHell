#include "normalbarrage.h"

NormalBarrage::NormalBarrage()
{
	m_instanceNumber++;
}

NormalBarrage::~NormalBarrage()
{
	m_instanceNumber--;
}
