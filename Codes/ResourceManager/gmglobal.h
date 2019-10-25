#ifndef GMGLOBAL_H
#define GMGLOBAL_H

#include <QObject>
#include <QMap>
#include "Image/sprite.h"

class GMGlobal : public QObject
{
    Q_OBJECT
public:
    explicit GMGlobal(QObject *parent = nullptr);

    Sprite &createSprite(QString name);
    Sprite &spriteByIndex(long long id);
    void removeSprite(long long id);

signals:

public slots:

private:
    QMap<long long, Sprite> m_sprites;
};

#endif // GMGLOBAL_H
