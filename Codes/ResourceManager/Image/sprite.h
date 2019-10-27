#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include <QList>

class Sprite : public QList<Image>
{
public:
    Sprite();
    Sprite(long long id, QString name);
    Sprite(const Sprite &another);
    ~Sprite();

    Sprite &operator=(const Sprite &another);
    Sprite &operator<<(const Image &image);

    long long id() const;

    QString name() const;
    void setName(const QString &name);

    void removeImage(int index = -1);

private:
    long long m_id;
    QString m_name;
};

#endif // SPRITE_H
