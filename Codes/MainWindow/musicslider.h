#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QSlider>

class MusicSlider : public QSlider
{
    Q_OBJECT

public:
    explicit MusicSlider(QWidget *parent = nullptr);
};

#endif // MUSICSLIDER_H
