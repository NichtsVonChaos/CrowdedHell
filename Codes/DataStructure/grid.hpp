#ifndef GRID_HPP
#define GRID_HPP

#include <iterator>
#include <limits>
#include <cmath>
#include <string>
#include <vector>
#include <cstring>
#include <exception>
#include <algorithm>

/*************************************************************************************************
 *                          Grid Data Structure
 * Author: Nihil
 * Last modified: Oct. 11, 2019
 *
 * Usage:
 *
 * 1. Initialize.
 *      The most effective initialize way may be:
 *          Grid<int> grid = {
 *              {12, 11, 7, -3},        | 12  11   7  -3|
 *              {-7, 0, 21, 10},    =>  | -7   0  21  10|
 *              {4, -1, -17, 5}};       |  4  -1 -17   5|
 *      Then if you call grid[1][3], you will get the value 10.
 *
 *      Anyway, you don't need provide all data such as:
 *          Grid<int> grid = {
 *              {12, 11, 7, -3},        | 12  11   7  -3|
 *              {-7},               =>  | -7   0   0   0|
 *              {4, -1}};               |  4  -1   0   0|
 *      Others will be filled by _Tp().
 *
 *      Well, if you want to define the empty object yourself, you can use:
 *          Grid<int> grid({
 *              {12, 0, 7, 3},          | 12  0   7   3|
 *              {7},                =>  |  7 -1  -1  -1|
 *              {4, 0}},                |  4  0  -1  -1|
 *              -1);
 *      or you can create grid at first and then fill data:
 *          Grid<int> grid(-1);
 *          grid = {
 *              {12, 0, 7, 3},          | 12  0   7   3|
 *              {7},                =>  |  7 -1  -1  -1|
 *              {4, 0}};                |  4  0  -1  -1|
 *
 *      Finaly, you can fill the whole grid with just one value:
 *          Gird<int> grid(4, 3, 1);
 *      It create a 4 rows and 3 columns grid then fill them with 1.
 *
 * 2. Access
 *      You can use the grid as a 2-D array easily.
 *      For some examples:
 *          Grid<int> grid = {
 *              {12, 11, 7, -3},        | 12  11   7  -3|
 *              {-7, 0, 21, 10},    =>  | -7   0  21  10|
 *              {4, -1, -17, 5}};       |  4  -1 -17   5|
 *          gird[1][2] = 10;
 *          int tail = grid[2][3];
 *
 *      Also, you can use 'at' function to access the grid. They are equivalent.
 *          grid.at(1, 2) = 10;
 *
 *      When the index out of range, it will throw std::out_of_range.
 *
 * 3. Traverse
 *      You can use the "range-based for loop" to traverse gird.
 *      That is:
 *          for(auto value : grid)
 *              std::cout << value;
 *
 *      Also, you can use iterators just like how you use the STL containers'.
 *
 * 4. Difference among realloc, reshape and resize
 *      Here are three examples to help you differ them (assuming that empty object is 0):
 *
 *          | 12  11   7  -3|   realloc(4, 3)   |  0   0   0|
 *          | -7   0  21  10|   ============>   |  0   0   0|
 *          |  4  -1 -17   5|                   |  0   0   0|
 *                                              |  0   0   0|
 *
 *          | 12  11   7  -3|   reshape(4, 3)   | 12  11   7|
 *          | -7   0  21  10|   ============>   | -3  -7   0|
 *          |  4  -1 -17   5|                   | 21  10   4|
 *                                              | -1 -17   5|
 *
 *          | 12  11   7  -3|   resize(4, 3)    | 12  11   7|
 *          | -7   0  21  10|   ===========>    | -7  0   21|
 *          |  4  -1 -17   5|                   |  4  -1 -17|
 *                                              |  0   0   0|
 *
 * 5. Equal
 *      When you call grid1 == grid2, firstly it will judge if their rows and columns is equal.
 *      Then it traverse two grid and check each element on the same position.
 *
 *      You can use your own function to judge if two element is equal by calling function "equal", such as:
 *          Grid<const char *> grid1 = {
 *              {"Hello", "World", "C++"},
 *              {"Java", "is", "rabbish"}};
 *          Grid<const char *> grid2 = {
 *              {"Hello", "World", "C++"},
 *              {"Java", "is", "rabbish"}};
 *          bool isEqual = grid1.equal(grid2, [](const char *const &first, const char *const &second) ->bool {
 *              return strcmp(first, second) == 0;
 *          });
 *      Generally, the variale "isEqual" will be true.
 *      Note the custom function must be the type bool(*)(const T &first, const T &second).
 *
 * 6. Replace and fill
 *      The function "replace" will replace a value to another value, and return how many values be replaced.
 *      The function "fill" will change all values of a specified range.
 *      Note the specified range (row1, col1, row2, col2) does not include row2 and col2.
 *      For a example, the range (1, 2, 2, 3) only include (1, 2).
 *
 * 7. Range
 *      The function "range" will return a copy of data on the specified range.
 *      Similarly, the specified range (row1, col1, row2, col2) does not include row2 and col2.
 *
 * 8. Sort, shuffle, and more
 *      It is easy. You can call STL algorithm what you want to use just like:
 *          std::shuffle(grid.begin(), grid.end());
 *
 * 9. Reverse
 *      To understand how the function "reverse(bool horizontal, bool vertical)" work, just see examples.
 *          | 12  11   7  -3|   reverse(true, false)    | -3   7  11  12|
 *          | -7   0  21  10|   ===================>    | 10  21   0  -7|
 *          |  4  -1 -17   5|                           |  5 -17   -1  4|
 *
 *          | 12  11   7  -3|   reverse(false, true)    |  4  -1 -17   5|
 *          | -7   0  21  10|   ===================>    | -7   0  21  10|
 *          |  4  -1 -17   5|                           | 12  11   7  -3|
 *
 *          | 12  11   7  -3|   reverse(true, true)     |  5 -17  -1   4|
 *          | -7   0  21  10|   ==================>     | 10  21   0  -7|
 *          |  4  -1 -17   5|                           | -3   7  11  12|
 *
 * 10. Swap
 *      If you want to swap two grid, do not do that:
 *          tmp = grid1;
 *          gird1 = grid2;
 *          gird2 = tmp;
 *      Please use:
 *          grid1.swap(grid2);
 *      or
 *          std::swap(grid1, grid2);
 *      instead.
 *
 *************************************************************************************************/

template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class Grid
{
public:
    typedef _Alloc allocator_type;
    typedef typename _Alloc::value_type value_type;
    typedef typename _Alloc::reference reference;
    typedef typename _Alloc::const_reference const_reference;
    typedef typename _Alloc::difference_type difference_type;
    typedef typename _Alloc::size_type size_type;
    typedef typename _Alloc::pointer pointer;
    typedef typename _Alloc::const_pointer const_pointer;
    class iterator;
    class const_iterator;

    typedef class _Grid_Row
    {
    public:
        _Grid_Row(pointer ptr, size_type columns);
        reference operator[](size_type column);
        const_reference operator[](size_type column) const;

    private:
        pointer m_ptr;
        size_type m_columns;
    } row_type;

    class iterator: public std::iterator<std::random_access_iterator_tag, value_type>
    {
    public:
        iterator() = delete;
        iterator(typename std::iterator<std::random_access_iterator_tag, value_type>::pointer ptr);
        iterator(const iterator &another);
        ~iterator();

        iterator& operator=(const iterator &another);
        bool operator==(const iterator &another) const;
        bool operator!=(const iterator &another) const;
        bool operator<(const iterator &another) const;
        bool operator>(const iterator &another) const;
        bool operator<=(const iterator &another) const;
        bool operator>=(const iterator &another) const;

        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
        iterator &operator+=(size_type size);
        iterator operator+(size_type size) const;
        template<typename __Tp, typename __Alloc>
        friend typename Grid<__Tp, __Alloc>::iterator operator+(typename Grid<__Tp, __Alloc>::size_type size, const typename Grid<__Tp, __Alloc>::iterator& iter);
        iterator &operator-=(size_type size);
        iterator operator-(size_type size) const;
        typename std::iterator<std::random_access_iterator_tag, value_type>::difference_type operator-(const iterator &another) const;

        typename std::iterator<std::random_access_iterator_tag, value_type>::reference operator*() const;
        typename std::iterator<std::random_access_iterator_tag, value_type>::pointer operator->() const;
        typename std::iterator<std::random_access_iterator_tag, value_type>::reference operator[](size_type offset) const;

        operator const_iterator() const;

    private:
        typename std::iterator<std::random_access_iterator_tag, value_type>::pointer _ptr;
    };

    class const_iterator: public std::iterator<std::random_access_iterator_tag, value_type>
    {
    public:
        const_iterator() = delete;
        const_iterator(typename const_iterator::const_point ptr);
        const_iterator(const const_iterator &another);
        const_iterator(const iterator &another);
        ~const_iterator();

        const_iterator& operator=(const const_iterator &another);
        const_iterator& operator=(const iterator &another);
        bool operator==(const const_iterator &another) const;
        bool operator!=(const const_iterator &another) const;
        bool operator<(const const_iterator &another) const;
        bool operator>(const const_iterator &another) const;
        bool operator<=(const const_iterator &another) const;
        bool operator>=(const const_iterator &another) const;

        const_iterator &operator++();
        const_iterator operator++(int);
        const_iterator &operator--();
        const_iterator operator--(int);
        const_iterator &operator+=(size_type size);
        const_iterator operator+(size_type size) const;
        template<typename __Tp, typename __Alloc>
        friend typename Grid<__Tp, __Alloc>::const_iterator operator+(typename Grid<__Tp, __Alloc>::size_type size, const typename Grid<__Tp, __Alloc>::const_iterator &iter);
        const_iterator &operator-=(size_type size);
        const_iterator operator-(size_type size) const;
        typename std::iterator<std::random_access_iterator_tag, value_type>::difference_type operator-(const const_iterator &another) const;

        const typename std::iterator<std::random_access_iterator_tag, value_type>::reference operator*() const;
        const typename std::iterator<std::random_access_iterator_tag, value_type>::pointer operator->() const;
        const typename std::iterator<std::random_access_iterator_tag, value_type>::reference operator[](size_type offset) const;

        operator iterator() = delete;
        operator const iterator() const;

    private:
        pointer _ptr;
    };

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Grid(const value_type &empty = value_type());
    Grid(const _Tp *const data, size_type rows, size_type cols, const value_type &empty = value_type());
    Grid(size_type rows ,size_type cols, const value_type &fill = value_type(), const value_type &empty = value_type());
    Grid(std::initializer_list<std::initializer_list<value_type> > data, const value_type &empty = value_type());
    Grid(const std::vector<std::vector<_Tp> > &data, const value_type &empty = value_type());
    Grid(const Grid<_Tp, _Alloc> &another);
    ~Grid() noexcept;

    Grid &operator=(std::initializer_list<std::initializer_list<value_type>> values);
    Grid &operator=(const Grid<_Tp, _Alloc> &another);
    bool operator==(const Grid<_Tp, _Alloc> &another) const;
    bool operator!=(const Grid<_Tp, _Alloc> &another) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    bool equal(const Grid<_Tp, _Alloc> &another, bool(*equalFunc)(const _Tp &first, const _Tp &second) = nullptr) const;

    row_type operator[](size_type col);
    const row_type operator[](size_type col) const;
    reference at(size_type row, size_type col);
    const_reference at(size_type row, size_type col) const;

    void clear() noexcept;
    void swap(Grid &another) noexcept;

    void realloc(size_type rows, size_type cols);
    void realloc(size_type rows, size_type cols, const value_type &value);
    void realloc(std::initializer_list<std::initializer_list<value_type>> values);

    void reshape(size_type rows, size_type cols);
    void resize(size_type rows, size_type cols);

    size_type rows() const noexcept;
    size_type columns() const noexcept;
    size_type count(const value_type &value) const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;
    void setEmptyObject(const value_type &object) noexcept;

    size_type replace(const value_type &oldValue, const value_type &newValue);
    size_type replace(const value_type &oldValue, const value_type &newValue, size_type row1, size_type col1, size_type row2, size_type col2);
    size_type replace(const value_type &oldValue, const value_type &newValue, iterator begin, iterator end);
    void fillRow(size_type index, const value_type &value);
    void fillColumn(size_type index, const value_type &value);
    void fillEmpty(const value_type &value);
    void fill(const value_type &value);
    void fill(const value_type &value, size_type row1, size_type col1, size_type row2, size_type col2);
    void fill(const value_type &value, iterator begin, iterator end);

    void reverse(bool horizontal = true, bool vertical = true);
    Grid<value_type> range(size_type row1, size_type col1, size_type row2, size_type col2);

    _Alloc get_allocator() const;

private:
    pointer m_grid;
    size_type m_rows, m_columns;
    allocator_type m_allocator;
    value_type m_empty;
};

template <class _Tp, class _Alloc = std::allocator<_Tp> >
void swap(Grid<_Tp, _Alloc> &grid_1, Grid<_Tp, _Alloc> &grid_2)
{
    grid_1.swap(grid_2);
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(const value_type &empty):
    m_grid(nullptr), m_rows(0), m_columns(0), m_empty(empty)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(const _Tp *const data, Grid<_Tp, _Alloc>::size_type rows, Grid<_Tp, _Alloc>::size_type cols, const Grid<_Tp, _Alloc>::value_type &empty):
    m_grid(nullptr), m_rows(rows), m_columns(cols), m_empty(empty)
{
    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    std::memcpy(m_grid, data, rows * cols * sizeof(_Tp));
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols, const typename Grid<_Tp, _Alloc>::value_type &fill, const value_type &empty):
    m_grid(nullptr), m_rows(rows), m_columns(cols), m_empty(empty)
{
    if(rows * cols == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < rows * cols; i++)
        m_grid[i] = fill;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(std::initializer_list<std::initializer_list<Grid<_Tp, _Alloc>::value_type> > data, const value_type &empty):
    m_grid(nullptr), m_rows(0), m_columns(0), m_empty(empty)
{
    m_rows = data.size();
    for(auto i : data)
    {
        if(i.size() > m_columns)
            m_columns = i.size();
    }

    if(m_rows * m_columns == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < m_rows * m_columns; i++)
        m_grid[i] = m_empty;

    size_type row = 0;
    for(auto i : data)
    {
        size_type column = 0;
        for(auto j : i)
        {
            m_grid[row * m_columns + column] = j;
            column++;
        }
        row++;
    }
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(const std::vector<std::vector<_Tp> > &data, const Grid<_Tp, _Alloc>::value_type &empty):
    m_grid(nullptr), m_rows(0), m_columns(0), m_empty(empty)
{
    m_rows = data.size();
    for(auto i : data)
    {
        if(i.size() > m_columns)
            m_columns = i.size();
    }

    if(m_rows * m_columns == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < m_rows * m_columns; i++)
        m_grid[i] = m_empty;

    size_type row = 0;
    for(auto i : data)
    {
        size_type column = 0;
        for(auto j : i)
        {
            m_grid[row * m_columns + column] = j;
            column++;
        }
        row++;
    }
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(const Grid<_Tp, _Alloc> &another):
    m_grid(nullptr), m_rows(0), m_columns(0)
{
    m_rows = another.rows();
    m_columns = another.columns();

    if(m_rows * m_columns == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_t i = 0; i < m_rows; i++)
        for(size_t j = 0; j < m_columns; j++)
            m_grid[i * m_columns + j] = another.at(i, j);

    m_empty = another.m_empty;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::~Grid() noexcept
{
    clear();
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc> &Grid<_Tp, _Alloc>::operator=(std::initializer_list<std::initializer_list<Grid::value_type> > values)
{
    m_rows = values.size();
    for(auto i : values)
    {
        if(i.size() > m_columns)
            m_columns = i.size();
    }

    if(m_rows * m_columns == 0)
        return *this;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < m_rows * m_columns; i++)
        m_grid[i] = m_empty;

    size_type row = 0;
    for(auto i : values)
    {
        size_type column = 0;
        for(auto j : i)
        {
            m_grid[row * m_columns + column] = j;
            column++;
        }
        row++;
    }

    return *this;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc> &Grid<_Tp, _Alloc>::operator=(const Grid<_Tp, _Alloc> &another)
{
    clear();

    m_rows = another.rows();
    m_columns = another.columns();

    if(m_rows * m_columns == 0)
        return *this;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_t i = 0; i < m_rows; i++)
        for(size_t j = 0; j < m_columns; j++)
            m_grid[i][j] = another.at(i, j);

    return *this;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::operator==(const Grid<_Tp, _Alloc> &another) const
{
    if(m_rows != another.rows())
        return false;

    if(m_columns != another.columns())
        return false;

    for(size_type i = 0; i < m_rows; i++)
        for(size_type j = 0; j < m_columns; j++)
            if(m_grid[i * m_columns + j] != another.at(i, j))
                return false;

    return true;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::operator!=(const Grid<_Tp, _Alloc> &another) const
{
    return !operator==(another);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::begin()
{
    return iterator(m_grid);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::begin() const
{
    return const_iterator(m_grid);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::cbegin() const
{
    return const_iterator(m_grid);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::end()
{
    return iterator(m_grid + size());
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::end() const
{
    return const_iterator(m_grid + size());
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::cend() const
{
    return const_iterator(m_grid + size());
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reverse_iterator Grid<_Tp, _Alloc>::rbegin()
{
    return reverse_iterator(iterator(m_grid + size() - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reverse_iterator Grid<_Tp, _Alloc>::rbegin() const
{
    return const_reverse_iterator(const_iterator(m_grid + size() - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reverse_iterator Grid<_Tp, _Alloc>::crbegin() const
{
    return const_reverse_iterator(const_iterator(m_grid + size() - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reverse_iterator Grid<_Tp, _Alloc>::rend()
{
    return reverse_iterator(iterator(m_grid - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reverse_iterator Grid<_Tp, _Alloc>::rend() const
{
    return const_reverse_iterator(const_iterator(m_grid - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reverse_iterator Grid<_Tp, _Alloc>::crend() const
{
    return const_reverse_iterator(const_iterator(m_grid - 1));
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::front()
{
    if(!m_grid)
        return m_empty;
    return at(0u, 0u);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::front() const
{
    if(!m_grid)
        return m_empty;
    return at(0u, 0u);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::back()
{
    if(!m_grid)
        return m_empty;
    return at(m_rows - 1, m_columns - 1);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::back() const
{
    if(!m_grid)
        return m_empty;
    return at(m_rows - 1, m_columns - 1);
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::equal(const Grid<_Tp, _Alloc> &another, bool(*equalFunc)(const _Tp &, const _Tp &)) const
{
    if(!equalFunc)
        return this->operator==(another);

    if(m_rows != another.rows())
        return false;

    if(m_columns != another.columns())
        return false;

    for(size_type i = 0; i < m_rows; i++)
        for(size_type j = 0; j < m_columns; j++)
            if(!equalFunc(m_grid[i * m_columns + j], another.at(i, j)))
                return false;

    return true;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::row_type Grid<_Tp, _Alloc>::operator[](typename Grid<_Tp, _Alloc>::size_type row)
{
    if(row >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row) + std::string(" of ") + std::to_string(m_rows));
    return row_type(m_grid + row * m_columns, m_columns);
}

template<typename _Tp, typename _Alloc>
const typename Grid<_Tp, _Alloc>::row_type Grid<_Tp, _Alloc>::operator[](typename Grid<_Tp, _Alloc>::size_type row) const
{
    if(row >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row) + std::string(" of ") + std::to_string(m_rows));
    return row_type(m_grid + row * m_columns, m_columns);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::at(typename Grid<_Tp, _Alloc>::size_type row, typename Grid<_Tp, _Alloc>::size_type col)
{
    if(row >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row) + std::string(" of ") + std::to_string(m_rows));
    if(col >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(col) + std::string(" of ") + std::to_string(m_columns));
    return m_grid[row * m_columns + col];
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::at(typename Grid<_Tp, _Alloc>::size_type row, typename Grid<_Tp, _Alloc>::size_type col) const
{
    if(row >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row) + std::string(" of ") + std::to_string(m_rows));
    if(col >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(col) + std::string(" of ") + std::to_string(m_columns));
    return m_grid[row * m_columns + col];
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::clear() noexcept
{
    if(m_grid != nullptr)
        m_allocator.deallocate(m_grid, m_rows * m_columns);
    m_grid = nullptr;
    m_rows = 0;
    m_columns = 0;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::swap(Grid &another) noexcept
{
    pointer tmp_grid = m_grid;
    m_grid = another.m_grid;
    another.m_grid = m_grid;

    size_type tmp_row = m_rows;
    m_rows = another.m_rows;
    another.m_rows = tmp_row;

    size_type tmp_col = m_columns;
    m_columns = another.m_columns;
    another.m_columns = tmp_col;

    allocator_type tmp_alloc = m_allocator;
    m_allocator = another.m_allocator;
    another.m_allocator = m_allocator;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols)
{
    clear();
    if(rows * cols == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < rows * cols; i++)
        m_grid[i] = m_empty;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols, const typename Grid<_Tp, _Alloc>::value_type &value)
{
    clear();
    if(rows * cols == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < rows * cols; i++)
        m_grid[i] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(std::initializer_list<std::initializer_list<typename Grid<_Tp, _Alloc>::value_type> > values)
{
    clear();
    m_rows = values.size();
    for(auto i : values)
    {
        if(i.size() > m_columns)
            m_columns = i.size();
    }

    if(m_rows * m_columns == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(m_rows * m_columns);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    size_type row = 0;
    for(auto i : values)
    {
        size_type column = 0;
        for(auto j : i)
        {
            m_grid[row * m_columns + column] = j;
            column++;
        }
        row++;
    }
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::reshape(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols)
{
    pointer old_grid = m_grid;
    size_type old_size = m_rows * m_columns;
    m_rows = rows;
    m_columns = cols;
    m_grid = nullptr;

    if(rows * cols == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    for(size_type i = 0; i < std::min(old_size, m_rows * m_columns); i++)
    {
        m_grid[i] = old_grid[i];
    }

    for(size_type i = std::min(old_size, m_rows * m_columns); i < m_rows * m_columns; i++)
    {
        m_grid[i] = m_empty;
    }

    if(m_grid != nullptr)
        m_allocator.deallocate(old_grid, old_size);
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::resize(Grid::size_type rows, Grid::size_type cols)
{
    pointer old_grid = m_grid;
    size_type old_rows = m_rows, old_columns = m_columns;
    m_rows = rows;
    m_columns = cols;
    m_grid = nullptr;

    if(rows * cols == 0)
        return;

    try
    {
        m_grid = m_allocator.allocate(rows * cols);
    }
    catch(const std::bad_alloc &e)
    {
        m_grid = nullptr;
        m_rows = 0;
        m_columns = 0;
        throw e;
    }

    fill(m_empty);

    for(size_type i = 0; i < std::min(old_rows, m_rows); i++)
        for(size_type j = 0; j < std::min(old_columns, m_columns); j++)
            m_grid[i * m_columns + j] = old_grid[i * old_columns + j];

    if(m_grid != nullptr)
        m_allocator.deallocate(old_grid, old_rows * old_columns);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::rows() const noexcept
{
    return m_rows;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::columns() const noexcept
{
    return m_columns;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::count(const typename Grid<_Tp, _Alloc>::value_type &value) const noexcept
{
    size_type count = 0;
    for(size_type i = 0; i < m_rows * m_columns; i++)
        if(m_grid[i] == value)
            count++;

    return count;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::size() const noexcept
{
    return m_rows * m_columns;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::max_size() const noexcept
{
    return std::numeric_limits<difference_type>::max();
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::empty() const noexcept
{
    return m_grid == nullptr || !m_rows || !m_columns;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::setEmptyObject(const typename Grid<_Tp, _Alloc>::value_type &object) noexcept
{
    m_empty = object;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue)
{
    for(size_type i = 0; i < m_rows * m_columns; i++)
        if(m_grid[i] == oldValue)
            m_grid[i] = newValue;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue, typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{
    if(row1 > row2)
    {
        row1 ^= row2;
        row2 ^= row1;
        row1 ^= row2;
    }

    if(col1 > col2)
    {
        col1 ^= col2;
        col2 ^= col1;
        col1 ^= col2;
    }

    if(row2 >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row2) + std::string(" of ") + std::to_string(m_rows));

    if(col2 >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(col2) + std::string(" of ") + std::to_string(m_columns));

    for(size_type row = row1; row < row2; row++)
        for(size_type col = col1; col < col2; col++)
            if(m_grid[row * m_columns + col] == oldValue)
                m_grid[row * m_columns + col] = newValue;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue, typename Grid<_Tp, _Alloc>::iterator begin, typename Grid<_Tp, _Alloc>::iterator end)
{
    for(;begin < end && begin < this->end(); begin++)
        if(*begin == oldValue)
            *begin = newValue;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fillRow(typename Grid<_Tp, _Alloc>::size_type index, const typename Grid<_Tp, _Alloc>::value_type &value)
{
    if(index >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(index) + std::string(" of ") + std::to_string(m_rows));
    for(size_type i = 0; i < m_columns; i++)
        m_grid[index * m_columns + i] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fillColumn(typename Grid<_Tp, _Alloc>::size_type index, const typename Grid<_Tp, _Alloc>::value_type &value)
{
    if(index >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(index) + std::string(" of ") + std::to_string(m_columns));
    for(size_type i = 0; i < m_rows; i++)
        m_grid[i * m_columns + index] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fillEmpty(const typename Grid<_Tp, _Alloc>::value_type &value)
{
    for(size_type i = 0; i < m_rows * m_columns; i++)
        if(m_grid[i] == m_empty)
            m_grid[i] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value)
{
    for(size_type i = 0; i < m_rows * m_columns; i++)
        m_grid[i] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value, typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{
    if(row1 > row2)
    {
        row1 ^= row2;
        row2 ^= row1;
        row1 ^= row2;
    }

    if(col1 > col2)
    {
        col1 ^= col2;
        col2 ^= col1;
        col1 ^= col2;
    }

    if(row2 >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row2) + std::string(" of ") + std::to_string(m_rows));

    if(col2 >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(col2) + std::string(" of ") + std::to_string(m_columns));

    for(size_type row = row1; row < row2; row++)
        for(size_type col = col1; col < col2; col++)
            m_grid[row * m_columns + col] = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value, typename Grid<_Tp, _Alloc>::iterator begin, typename Grid<_Tp, _Alloc>::iterator end)
{
    for(;begin < end && begin < this->end(); begin++)
        *begin = value;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::reverse(bool horizontal, bool vertical)
{
    if(!horizontal && !vertical)
        return;
    else if(!vertical)
    {
        value_type tmp = m_empty;
        for(size_type i = 0; i < m_rows; i++)
            for(size_type j = 0; j < m_columns / 2; j++)
            {
                tmp = m_grid[i * m_columns + j];
                m_grid[i * m_columns + j] = m_grid[(i + 1) * m_columns - j - 1];
                m_grid[(i + 1) * m_columns - j - 1] = tmp;
            }
    }
    else if(!horizontal)
    {
        value_type tmp = m_empty;
        for(size_type i = 0; i < m_rows / 2; i++)
            for(size_type j = 0; j < m_columns; j++)
            {
                tmp = m_grid[i * m_columns + j];
                m_grid[i * m_columns + j] = m_grid[(m_columns - i - 1) * m_columns + j];
                m_grid[(m_columns - i - 1) * m_columns + j] = tmp;
            }
    }
    else
    {
        value_type tmp = m_empty;
        for(size_type i = 0; i < m_rows * m_columns / 2; i++)
        {
            tmp = m_grid[i];
            m_grid[i] = m_grid[m_rows * m_columns - i - 1];
            m_grid[m_rows * m_columns - i - 1] = tmp;
        }
    }
}

template<typename _Tp, typename _Alloc>
Grid<typename Grid<_Tp, _Alloc>::value_type> Grid<_Tp, _Alloc>::range(typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{
    if(row1 > row2)
    {
        row1 ^= row2;
        row2 ^= row1;
        row1 ^= row2;
    }

    if(col1 > col2)
    {
        col1 ^= col2;
        col2 ^= col1;
        col1 ^= col2;
    }

    if(row2 >= m_rows)
        throw std::out_of_range(std::string("Grid<> : Out of range at row ") + std::to_string(row2) + std::string(" of ") + std::to_string(m_rows));

    if(col2 >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(col2) + std::string(" of ") + std::to_string(m_columns));

    Grid<typename Grid<_Tp, _Alloc>::value_type> newGrid(row2 - row1, col2 - col1, m_empty);
    for(size_type row = 0; row < row2 - row1; row++)
        for(size_type col = 0; col < col2 - col1; col++)
            newGrid[row][col] = at(row + row1, col + col1);
}

template<typename _Tp, typename _Alloc>
_Alloc Grid<_Tp, _Alloc>::get_allocator() const
{
    return _Alloc();
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::iterator(typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::pointer ptr)
{
    _ptr = ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::iterator(const typename Grid<_Tp, _Alloc>::iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::~iterator()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator=(const typename Grid<_Tp, _Alloc>::iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator==(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr == another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator!=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr != another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator<(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr < another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator>(const typename  Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr > another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator<=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr <= another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator>=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr >= another._ptr;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator++()
{
    _ptr++;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator++(int)
{
    typename Grid<_Tp, _Alloc>::iterator old = *this;
    _ptr++;
    return old;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator--()
{
    _ptr--;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator--(int)
{
    typename Grid<_Tp, _Alloc>::iterator old = *this;
    _ptr--;
    return old;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator+=(typename Grid<_Tp, _Alloc>::size_type size)
{
    _ptr += size;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator+(typename Grid<_Tp, _Alloc>::size_type size) const
{
    return typename Grid<_Tp, _Alloc>::iterator(_ptr + size);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator operator+(typename Grid<_Tp, _Alloc>::size_type size, const typename Grid<_Tp, _Alloc>::iterator &iter)
{
    return iter + size;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator-=(typename Grid<_Tp, _Alloc>::size_type size)
{
    _ptr -= size;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator-(typename Grid<_Tp, _Alloc>::size_type size) const
{
    return typename Grid<_Tp, _Alloc>::iterator(_ptr - size);
}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::difference_type Grid<_Tp, _Alloc>::iterator::operator-(const typename Grid<_Tp, _Alloc>::iterator &another) const
{
    return _ptr - another._ptr;
}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::iterator::operator*() const
{
    return *_ptr;
}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::pointer Grid<_Tp, _Alloc>::iterator::operator->() const
{
    return _ptr;
}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::iterator::operator[](typename Grid<_Tp, _Alloc>::size_type offset) const
{
    return *(_ptr + offset);
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::operator Grid<_Tp, _Alloc>::const_iterator() const
{
    return Grid<_Tp, _Alloc>::const_iterator(_ptr);
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(typename const_iterator::const_point ptr)
{
    _ptr = ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(const typename Grid<_Tp, _Alloc>::const_iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(const typename Grid<_Tp, _Alloc>::iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::~const_iterator()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator=(const typename Grid<_Tp, _Alloc>::const_iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator=(const typename Grid<_Tp, _Alloc>::iterator &another)
{
    _ptr = another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator==(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr == another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator!=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr != another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator<(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr < another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator>(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr > another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator<=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr <= another._ptr;
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator>=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr >= another._ptr;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator++()
{
    _ptr++;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator++(int)
{
    typename Grid<_Tp, _Alloc>::const_iterator old = *this;
    _ptr++;
    return old;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator--()
{
    _ptr--;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator--(int)
{
    typename Grid<_Tp, _Alloc>::const_iterator old = *this;
    _ptr--;
    return old;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator+=(Grid<_Tp, _Alloc>::size_type size)
{
    _ptr += size;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator+(Grid<_Tp, _Alloc>::size_type size) const
{
    return typename Grid<_Tp, _Alloc>::const_iterator(_ptr + size);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator operator+(typename Grid<_Tp, _Alloc>::size_type size, const typename Grid<_Tp, _Alloc>::const_iterator &iter)
{
    return iter + size;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator-=(typename Grid<_Tp, _Alloc>::size_type size)
{
    _ptr -= size;
    return *this;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator-(typename Grid<_Tp, _Alloc>::size_type size) const
{
    return typename Grid<_Tp, _Alloc>::const_iterator(_ptr - size);
}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::difference_type Grid<_Tp, _Alloc>::const_iterator::operator-(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{
    return _ptr - another._ptr;
}

template<typename _Tp, typename _Alloc>
const typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::const_iterator::operator*() const
{
    return *_ptr;
}

template<typename _Tp, typename _Alloc>
const typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::pointer Grid<_Tp, _Alloc>::const_iterator::operator->() const
{
    return _ptr;
}

template<typename _Tp, typename _Alloc>
const typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::const_iterator::operator[](typename Grid<_Tp, _Alloc>::size_type offset) const
{
    return *(_ptr + offset);
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::operator const Grid<_Tp, _Alloc>::iterator() const
{
    return Grid<_Tp, _Alloc>::iterator(_ptr);
}


template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::_Grid_Row::_Grid_Row(typename Grid<_Tp, _Alloc>::pointer ptr, typename Grid<_Tp, _Alloc>::size_type columns)
{
    m_ptr = ptr;
    m_columns = columns;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::_Grid_Row::operator[](typename Grid<_Tp, _Alloc>::size_type column)
{
    if(column >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(column) + std::string(" of ") + std::to_string(m_columns));
    else
        return m_ptr[column];
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::_Grid_Row::operator[](typename Grid<_Tp, _Alloc>::size_type column) const
{
    if(column >= m_columns)
        throw std::out_of_range(std::string("Grid<> : Out of range at column ") + std::to_string(column) + std::string(" of ") + std::to_string(m_columns));
    else
        return m_ptr[column];
}

#endif // GRID_HPP
