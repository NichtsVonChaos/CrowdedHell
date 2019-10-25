#include "sprite.h"

Sprite::Sprite():
    m_id(-1)
{

}

Sprite::Sprite(long long id, QString name):
    m_id(id), m_name(name)
{

}

Sprite::Sprite(const Sprite &another):
    QList(another), m_id(another.m_id), m_name(another.m_name)
{

}

Sprite &Sprite::operator=(const Sprite &another)
{
    QList::operator=(another);
    m_id = another.m_id;
    m_name = another.m_name;
}

Sprite &Sprite::operator <<(const Image &image)
{
    QList::operator<<(image);
    return *this;
}

long long Sprite::id() const
{
    return m_id;
}

QString Sprite::name() const
{
    return m_name;
}

void Sprite::setName(const QString &name)
{
    m_name = name;
}
