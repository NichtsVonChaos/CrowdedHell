#ifndef UTILITY_H
#define UTILITY_H

#include <QtGlobal>
#include <QString>

#if defined(Q_OS_WIN)
#include <objbase.h>
#elif defined(Q_OS_LINUX)
#include <uuid/uuid.h>
#endif

class Utility
{
public:
    static QString generateUUID();

private:
    Utility() = delete;
    Utility(const Utility &) = delete;
    Utility(Utility &&) = delete;
    Utility &operator =(const Utility &) = delete;
};

#endif // UTILITY_H
