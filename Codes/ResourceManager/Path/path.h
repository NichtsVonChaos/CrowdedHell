#ifndef PATH_H
#define PATH_H

#include <QString>
#include <QList>

struct PathPoint
{
    double x;
    double y;
    double speed;
};

class Path : QList<PathPoint>
{
public:
    Path();

private:
    long long id;
    QString name;
    bool m_closed, m_smooth;
    int m_precision;
};

#endif // PATH_H
