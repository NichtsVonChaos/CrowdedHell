#ifndef IMAGE_H
#define IMAGE_H

#include "Codes/DataStructure/grid.hpp"
#include "mask.h"

typedef unsigned char Color;

struct Pixel
{
public:
    Pixel(Color r = 0, Color g = 0, Color b = 0, Color a = 0);
    Pixel(unsigned int abgr);
    Color r, g, b, a;
};

class Image : public Grid<Pixel>
{
public:
    Image();
};

#endif // IMAGE_H
