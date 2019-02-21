#ifndef NORMALBARRAGE_H
#define NORMALBARRAGE_H

#include "metagmobject.h"

class NormalBarrage : public MetaGMObject
{
	Q_OBJECT

public:
	NormalBarrage(ResourceManager *resourceManager, int level);
	virtual ~NormalBarrage() override;

	virtual int getInteger(int attribute) override;
	virtual double getDouble(int attribute) override;
	virtual bool getBoolean(int attribute) override;
	virtual QString getString(int attribute) override;
	virtual void *getPointer(int attribute) override;

	virtual void setInteger(int attribute, int value) override;
	virtual void setDouble(int attribute, double value) override;
	virtual void setBoolean(int attribute, bool value) override;
	virtual void setString(int attribute, QString value) override;
	virtual void setPointer(int attribute, void *value) override;

	virtual void GMEvent_BeginStep() override;
	virtual void GMEvent_Alarm(unsigned int number) override;
	virtual void GMEvent_Step() override;
	virtual void GMEvent_Collision(MetaGMObject* object) override;
	virtual void GMEvent_EndStep() override;
	virtual void GMEvent_Draw(QWidget* widget) override;

public slots:
	virtual void infer(QTime time) override;
	virtual void prevFrame() override;
	virtual void nextFrame() override;
	virtual void prev(unsigned int frames) override;
	virtual void next(unsigned int frames) override;

protected:
	int m_level;
};

#endif // NORMALBARRAGE_H
