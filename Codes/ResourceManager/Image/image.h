#ifndef IMAGE_H
#define IMAGE_H

#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QFile>

#include "Codes/DataStructure/grid.hpp"
#include "mask.h"

class Image : public QObject
{
    Q_OBJECT

public:
    explicit Image(QObject *parent = nullptr);
    Image(const QString &filePath, QObject *parent = nullptr);
    Image(const QImage &qImage, QObject *parent = nullptr);
    Image(const Image &another);
    ~Image() override;

    Image &operator=(const Image &another);

    Mask &mask();
    const Mask &mask() const;
    QImage &qImage();
    const QImage &qImage() const;
    QPixmap qPixmap() const;

    void generateMask(unsigned char tolerance = 1);

    void remove();

private:
    Mask m_mask;
    QString m_imageFile;
    QImage m_qImage;
};

#endif // IMAGE_H
