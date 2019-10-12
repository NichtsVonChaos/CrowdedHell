#include "image.h"

Image::Image(QObject *parent):
    QObject(parent), Grid(0)
{

}

Image::Image(const unsigned int *const data, size_t rows, size_t cols, QObject *parent):
    QObject(parent), Grid(data, rows, cols, 0), mask(rows, cols)
{
    for(size_t i = 0; i < rows * cols; i++)
        mask.at(i) = bool(alpha(at(i)));
}

Image::~Image()
{

}

unsigned char Image::red(unsigned int agbr) noexcept
{
    return static_cast<unsigned char>(agbr & 0xFFU);
}

unsigned char Image::green(unsigned int agbr) noexcept
{
    return static_cast<unsigned char>((agbr & 0xFF00U) >> 8);
}

unsigned char Image::blue(unsigned int agbr) noexcept
{
    return static_cast<unsigned char>((agbr & 0xFF0000U) >> 16);
}

unsigned char Image::alpha(unsigned int agbr) noexcept
{
    return static_cast<unsigned char>((agbr & 0xFF000000U) >> 24);
}

unsigned int Image::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
{
    return (unsigned(a) << 24u) | (unsigned(b) << 16u) | (unsigned(g) << 8u) | unsigned(r);
}
