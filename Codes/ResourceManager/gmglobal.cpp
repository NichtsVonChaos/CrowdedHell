#include "gmglobal.h"

GMGlobal::GMGlobal(QObject *parent) : QObject(parent)
{
    m_sprites.insert(-1, Sprite(-1, "(No Sprite)"));
}

Sprite &GMGlobal::spriteByIndex(long long id)
{
    if(m_sprites.contains(id))
        return m_sprites[id];
    else
        return m_sprites[-1];
}
