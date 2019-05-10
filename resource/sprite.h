#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include "datastructrue/grid.hpp"

class Image : public QObject
{
	Q_OBJECT
public:
	explicit Image(QObject *parent = nullptr);

private:
	Grid<bool> *m_mask;
};

class Sprite : public QObject
{
	Q_OBJECT
public:
	explicit Sprite(QObject *parent = nullptr);

signals:

public slots:

private:
	int m_index;
	int m_image_number;
	QString m_name;
	QPoint m_origin;
};

#endif // SPRITE_H
