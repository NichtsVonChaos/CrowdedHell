#include "mask.h"

Mask::Mask()
{

}

Mask::Mask(const bool *const data, size_t rows, size_t cols):
    Grid(data, rows, cols, false)
{

}

Mask::Mask(std::initializer_list<std::initializer_list<bool> > data):
    Grid(data, false)
{

}

Mask::Mask(const std::vector<std::vector<bool> > &data):
    Grid(data, false)
{

}

Mask::Mask(Grid::size_type rows, Grid::size_type cols, bool fill):
    Grid(rows, cols, fill, false)
{

}

Mask::Mask(const Grid<bool> &another):
    Grid(another)
{

}

QDebug operator<<(QDebug qDbg, const Mask &mask)
{
    qDbg.nospace() << QString("Mask -- row:") + QString::number(mask.rows()) + QString(" cols:") + QString::number(mask.columns()) << endl;
    for(size_t i = 0; i < mask.rows(); i++)
    {
        qDbg.nospace() << "|";
        for(size_t j = 0; j < mask.columns(); j++)
        {
            qDbg.nospace() << " " << int(mask.at(i, j));
        }
        qDbg.nospace() << "|" << endl;
    }

    return qDbg.nospace();
}
