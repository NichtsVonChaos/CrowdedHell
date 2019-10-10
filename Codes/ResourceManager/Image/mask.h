#ifndef MASK_H
#define MASK_H

#include "Codes/DataStructure/grid.hpp"
#include <QDebug>
#include <QString>

class Mask : public Grid<bool>
{
public:
    Mask();
    Mask(const bool *const data, size_t rows, size_t cols);
    Mask(size_t rows ,size_t cols, bool fill = false);
    Mask(std::initializer_list<std::initializer_list<bool> > data);
    Mask(const std::vector<std::vector<bool> > &data);
    Mask(const Grid<bool> &another);

    friend QDebug &operator<< (QDebug &qDbg, const Mask &mask);
};

QDebug operator<< (QDebug qDbg, const Mask &mask);

#endif // MASK_H
