#ifndef NORMALBARRAGE_H
#define NORMALBARRAGE_H

#include "metagmobject.h"

class NormalBarrage : public MetaGMObject
{
	Q_OBJECT

public:
	NormalBarrage(int level);
	virtual ~NormalBarrage();

	virtual int getInteger(int attribute);
	virtual double getDouble(int attribute);
	virtual bool getBoolean(int attribute);
	virtual QString getString(int attribute);
	virtual void *getPointer(int attribute);

	virtual void setInteger(int attribute, int value);
	virtual void setDouble(int attribute, double value);
	virtual void setBoolean(int attribute, bool value);
	virtual void setString(int attribute, QString value);
	virtual void setPointer(int attribute, void *value);

	virtual void GMEvent_BeginStep();
	virtual void GMEvent_Alarm(unsigned int number);
	virtual void GMEvent_Step();
	virtual void GMEvent_Collision(MetaGMObject* object);
	virtual void GMEvent_EndStep();
	virtual void GMEvent_Draw(QWidget* widget);

public slots:
	virtual void infer(QTime time);
	virtual void prevFrame();
	virtual void nextFrame();
	virtual void prev(unsigned int frames);
	virtual void next(unsigned int frames);

protected:
	int m_level;
};

#endif // NORMALBARRAGE_H
