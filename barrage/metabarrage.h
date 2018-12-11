#ifndef METABARRAGE_H
#define METABARRAGE_H

#include <QMainWindow>
#include <QString>
#include <QTime>

/**
 * @brief The MetaBarrage class
 * Meta object of all kinds of barrages.
 * It is made as a state machine.
 */
class MetaBarrage : public QObject
{
	Q_OBJECT

public:
	virtual ~MetaBarrage() = 0;
	virtual int getInteger(int attribute) = 0;
	virtual double getDouble(int attribute) = 0;
	virtual bool getBoolean(int attribute) = 0;
	virtual QString getString(int attribute) = 0;
	virtual void *getPointer(int attribute) = 0;

	virtual void setInteger(int attribute, int value) = 0;
	virtual void setDouble(int attribute, double value) = 0;
	virtual void setBoolean(int attribute, bool value) = 0;
	virtual void setString(int attribute, QString value) = 0;
	virtual void setPointer(int attribute, void *value) = 0;

	static void restart();

public slots:
	virtual void infer(QTime time) = 0;
	virtual void prevFrame() = 0;
	virtual void nextFrame() = 0;

protected:
	static int m_barrageNumber;
	static double m_globalPlaySpeed;
};

#endif // METABARRAGE_H
