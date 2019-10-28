#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

template<typename T>
inline T limit(const T &value, const T &min, const T &max)
{
    if(value < min)
        return min;
    else if(value > max)
        return max;
    else
        return value;
};

#endif // CUSTOMFUNCTIONS_H
