#include "image.h"

Image::Image(QObject *parent):
    QObject(parent)
{

}

Image::Image(const QString &filePath, QObject *parent):
    QObject(parent), m_qImage(filePath)
{

}

Image::Image(const QImage &qImage, QObject *parent):
    QObject(parent), m_qImage(qImage)
{
    generateMask();
}

Image::Image(const Image &another):
    QObject(another.parent()), m_mask(another.mask()), m_qImage(another.qImage())
{

}

Image::~Image()
{

}

Image &Image::operator=(const Image &another)
{
    m_mask = another.mask();
    m_qImage = another.qImage();
    return *this;
}

Mask &Image::mask()
{
    return m_mask;
}

const Mask &Image::mask() const
{
    return m_mask;
}

QImage &Image::qImage()
{
    return m_qImage;
}

const QImage &Image::qImage() const
{
    return m_qImage;
}

QPixmap Image::qPixmap() const
{
    return QPixmap::fromImage(qImage());
}

void Image::generateMask(unsigned char tolerance)
{
    m_mask.realloc(size_t(m_qImage.height()), size_t(m_qImage.width()));
    for(int i = 0; i < m_qImage.height(); ++i)
        for(int j = 0; j < m_qImage.width(); ++j)
            m_mask.at(size_t(i), size_t(j)) = (m_qImage.pixelColor(j, i).alpha() >= tolerance);
}
