#include "image.h"

Image::Image()
{

}

Pixel::Pixel(Color r, Color g, Color b, Color a):
    r(r), g(g), b(b), a(a)
{

}

Pixel::Pixel(unsigned int abgr)
{
    Color *color = reinterpret_cast<Color *>(&abgr);
    r = color[0];
    g = color[1];
    b = color[2];
    a = color[3];
}
