#ifndef IMAGE_H
#define IMAGE_H

#include <QColor>

#include "Codes/DataStructure/grid.hpp"
#include "mask.h"

class Image : public Grid<unsigned int>, public QObject
{
    Q_OBJECT

public:
    explicit Image(QObject *parent = nullptr);
    Image(const unsigned int *const data, size_t rows, size_t cols, QObject *parent = nullptr);
    ~Image();

    inline static unsigned char red(unsigned int agbr) noexcept;
    inline static unsigned char green(unsigned int agbr) noexcept;
    inline static unsigned char blue(unsigned int agbr) noexcept;
    inline static unsigned char alpha(unsigned int agbr) noexcept;
    inline static unsigned int color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) noexcept;

private:
    Mask mask;
};

#endif // IMAGE_H
