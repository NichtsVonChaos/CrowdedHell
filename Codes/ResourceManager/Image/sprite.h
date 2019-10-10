#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include <QList>

class Sprite : public QList<Image>
{
public:
    Sprite();
};

#endif // SPRITE_H
