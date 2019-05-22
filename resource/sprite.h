#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include "datastructrue/grid.hpp"
#include "opencv2/opencv.hpp"

extern template
class Grid<bool>;

class Image : public QObject
{
	Q_OBJECT
public:
	explicit Image(QObject *parent = nullptr);

	static const int BLUE = 0;
	static const int GREEN = 1;
	static const int RED = 2;
	static const int ALPHA = 3;

private:
	Grid<bool> *m_mask;

	QString m_filePath;
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
