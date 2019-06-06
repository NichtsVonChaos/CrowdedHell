#ifndef GRID_HPP
#define GRID_HPP

#include <iterator>
#include <limits>

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
	typedef typename _Alloc::const_point const_point;
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

	class iterator:public std::iterator<std::random_access_iterator_tag, value_type>
	{
	public:
		iterator();
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
		friend iterator operator+(size_type size, const iterator& iter);
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
		typedef const typename std::iterator<std::random_access_iterator_tag, value_type>::pointer const_pointer;
		typedef const typename std::iterator<std::random_access_iterator_tag, value_type>::reference const_reference;

		const_iterator();
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
		friend const_iterator operator+(size_type size, const const_iterator &iter);
		const_iterator &operator-=(size_type size);
		const_iterator operator-(size_type size) const;
		typename std::iterator<std::random_access_iterator_tag, value_type>::difference_type operator-(const const_iterator &another) const;

		typename const_iterator::const_reference operator*() const;
		typename const_iterator::const_point operator->() const;
		typename const_iterator::const_reference operator[](size_type offset) const;

		operator iterator() = delete;

	private:
		pointer _ptr;
	};

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	Grid();
	Grid(size_type rows, size_type cols);
	Grid(size_type rows ,size_type cols, const value_type &value);
	Grid(std::initializer_list<std::initializer_list<value_type>> values);
	Grid(const Grid<_Tp, _Alloc> &another);
	~Grid();

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


	row_type operator[](size_type col);
	const row_type operator[](size_type col) const;
	reference at(size_type row, size_type col);
	const_reference at(size_type row, size_type col) const;

	void clear();
	void swap(Grid &another);

	void realloc(size_type rows, size_type cols);
	void realloc(size_type rows, size_type cols, const value_type &value);
	void realloc(std::initializer_list<std::initializer_list<value_type>> values);

	void reshape(size_type rows, size_type cols);

	size_type rows() const;
	size_type columns() const;
	size_type count(const value_type &value) const;
	size_type size() const;
	size_type max_size() const;
	bool empty() const;

	size_type replace(const value_type &oldValue, const value_type &newValue);
	size_type replace(const value_type &oldValue, const value_type &newValue, size_type row1, size_type col1, size_type row2, size_type col2);
	size_type replace(const value_type &oldValue, const value_type &newValue, iterator begin, iterator end);
	void fillRow(size_type index, const value_type &value);
	void fillColumn(size_type index, const value_type &value);
	void fill(const value_type &value);
	void fill(const value_type &value, size_type row1, size_type col1, size_type row2, size_type col2);
	void fill(const value_type &value, iterator begin, iterator end);

	void shuffle();
	void reverse(bool horizontal = true, bool vertical = true);
	Grid<value_type> range(size_type row1, size_type col1, size_type row2, size_type col2);

	_Alloc get_allocator() const;

private:
	pointer m_grid;
	size_type m_rows, m_columns;
	allocator_type m_allocator;
};

template <class _Tp, class _Alloc = std::allocator<_Tp> >
void swap(Grid<_Tp, _Alloc> &grid_1, Grid<_Tp, _Alloc> &grid_2)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid():
	m_grid(nullptr), m_columns(0), m_rows(0)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols):
	m_grid(nullptr), m_rows(rows), m_columns(cols)
{
	if(rows * cols == 0)
		return;

	m_grid = m_allocator.allocate(rows * cols);
	for(size_type i = 0; i < rows * cols; i++)
		m_grid[i] = value_type();
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols, const typename Grid<_Tp, _Alloc>::value_type &value):
	m_grid(nullptr), m_rows(rows), m_columns(cols)
{
	if(rows * cols == 0)
		return;

	m_grid = m_allocator.allocate(rows * cols);
	for(size_type i = 0; i < rows * cols; i++)
		m_grid[i] = value;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::Grid(std::initializer_list<std::initializer_list<Grid<_Tp, _Alloc>::value_type> > values):
	m_grid(nullptr), m_rows(0), m_columns(0)
{
	m_rows = values.size();
	for(auto i : values)
	{
		if(i.size() > m_columns)
			m_columns = i.size();
	}

	if(m_rows * m_columns == 0)
		return;

	m_grid = m_allocator.allocate(m_rows * m_columns);

	int row = 0;
	for(auto i : values)
	{
		int column = 0;
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

	m_grid = m_allocator.allocate(m_rows * m_columns);

	for(size_t i = 0; i < m_rows; i++)
		for(size_t j = 0; j < m_columns; j++)
			m_grid[i][j] = another.at(i, j);
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::~Grid()
{
	clear();
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc> &Grid<_Tp, _Alloc>::operator=(const Grid<_Tp, _Alloc> &another)
{
	clear();

	m_rows = another.rows();
	m_columns = another.columns();

	if(m_rows * m_columns == 0)
		return;

	m_grid = m_allocator.allocate(m_rows * m_columns);

	for(size_t i = 0; i < m_rows; i++)
		for(size_t j = 0; j < m_columns; j++)
			m_grid[i][j] = another.at(i, j);
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::operator==(const Grid<_Tp, _Alloc> &another) const
{
	if(m_rows != another.rows())
		return false;

	if(m_columns != another.columns())
		return false;

	for(int i = 0; i < m_rows; i++)
		for(int j = 0; j < m_columns; j++)
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
	return at(0u, 0u);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::front() const
{
	return at(0u, 0u);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::back()
{
	return at(m_rows - 1, m_columns - 1);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::back() const
{
	return at(m_rows - 1, m_columns - 1);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::row_type Grid<_Tp, _Alloc>::operator[](typename Grid<_Tp, _Alloc>::size_type row)
{
	return row_type(m_grid + row * m_columns, m_columns);
}

template<typename _Tp, typename _Alloc>
const typename Grid<_Tp, _Alloc>::row_type Grid<_Tp, _Alloc>::operator[](typename Grid<_Tp, _Alloc>::size_type row) const
{
	return row_type(m_grid + row * m_columns, m_columns);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::reference Grid<_Tp, _Alloc>::at(typename Grid<_Tp, _Alloc>::size_type row, typename Grid<_Tp, _Alloc>::size_type col)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::at(typename Grid<_Tp, _Alloc>::size_type row, typename Grid<_Tp, _Alloc>::size_type col) const
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::clear()
{
	if(m_grid != nullptr)
		m_allocator.deallocate(m_grid, m_rows * m_columns);
	m_grid = nullptr;
	m_rows = 0;
	m_columns = 0;
}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::swap(Grid &another)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols, const typename Grid<_Tp, _Alloc>::value_type &value)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::realloc(std::initializer_list<std::initializer_list<typename Grid<_Tp, _Alloc>::value_type> > values)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::reshape(typename Grid<_Tp, _Alloc>::size_type rows, typename Grid<_Tp, _Alloc>::size_type cols)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::rows() const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::columns() const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::count(const typename Grid<_Tp, _Alloc>::value_type &value) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::size() const
{
	return m_rows * m_columns;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::max_size() const
{
	return std::numeric_limits<difference_type>::max();
}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::empty() const
{
	return m_grid == nullptr || !m_rows || !m_columns;
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue, typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::size_type Grid<_Tp, _Alloc>::replace(const typename Grid<_Tp, _Alloc>::value_type &oldValue, const typename Grid<_Tp, _Alloc>::value_type &newValue, typename Grid<_Tp, _Alloc>::iterator begin, typename Grid<_Tp, _Alloc>::iterator end)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fillRow(typename Grid<_Tp, _Alloc>::size_type index, const typename Grid<_Tp, _Alloc>::value_type &value)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fillColumn(typename Grid<_Tp, _Alloc>::size_type index, const typename Grid<_Tp, _Alloc>::value_type &value)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value, typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::fill(const typename Grid<_Tp, _Alloc>::value_type &value, typename Grid<_Tp, _Alloc>::iterator begin, typename Grid<_Tp, _Alloc>::iterator end)
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::shuffle()
{

}

template<typename _Tp, typename _Alloc>
void Grid<_Tp, _Alloc>::reverse(bool horizontal, bool vertical)
{

}

template<typename _Tp, typename _Alloc>
Grid<typename Grid<_Tp, _Alloc>::value_type> Grid<_Tp, _Alloc>::range(typename Grid<_Tp, _Alloc>::size_type row1, typename Grid<_Tp, _Alloc>::size_type col1, typename Grid<_Tp, _Alloc>::size_type row2, typename Grid<_Tp, _Alloc>::size_type col2)
{

}

template<typename _Tp, typename _Alloc>
_Alloc Grid<_Tp, _Alloc>::get_allocator() const
{
	return _Alloc();
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::iterator()
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::iterator(typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::pointer ptr)
{
	_ptr = ptr;
}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::iterator(const typename Grid<_Tp, _Alloc>::iterator &another)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::~iterator()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator=(const typename Grid<_Tp, _Alloc>::iterator &another)
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator==(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator!=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator<(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator>(const typename  Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator<=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::iterator::operator>=(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator++()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator++(int)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator--()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator--(int)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator+=(typename Grid<_Tp, _Alloc>::size_type size)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator+(typename Grid<_Tp, _Alloc>::size_type size) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator operator+(typename Grid<_Tp, _Alloc>::size_type size, const typename Grid<_Tp, _Alloc>::iterator &iter)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator &Grid<_Tp, _Alloc>::iterator::operator-=(typename Grid<_Tp, _Alloc>::size_type size)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::iterator Grid<_Tp, _Alloc>::iterator::operator-(typename Grid<_Tp, _Alloc>::size_type size) const
{

}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::difference_type Grid<_Tp, _Alloc>::iterator::operator-(const typename Grid<_Tp, _Alloc>::iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::iterator::operator*() const
{

}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::pointer Grid<_Tp, _Alloc>::iterator::operator->() const
{

}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::reference Grid<_Tp, _Alloc>::iterator::operator[](typename Grid<_Tp, _Alloc>::size_type offset) const
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::iterator::operator const_iterator() const
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator()
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(typename const_iterator::const_point ptr)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(const typename Grid<_Tp, _Alloc>::const_iterator &another)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::const_iterator(const typename Grid<_Tp, _Alloc>::iterator &another)
{

}

template<typename _Tp, typename _Alloc>
Grid<_Tp, _Alloc>::const_iterator::~const_iterator()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator=(const typename Grid<_Tp, _Alloc>::const_iterator &another)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator=(const typename Grid<_Tp, _Alloc>::iterator &another)
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator==(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator!=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator<(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator>(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator<=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
bool Grid<_Tp, _Alloc>::const_iterator::operator>=(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator++()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator++(int)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator--()
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator--(int)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator+=(Grid<_Tp, _Alloc>::size_type size)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator+(Grid<_Tp, _Alloc>::size_type size) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator operator+(typename Grid<_Tp, _Alloc>::size_type size, const typename Grid<_Tp, _Alloc>::const_iterator &iter)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator &Grid<_Tp, _Alloc>::const_iterator::operator-=(typename Grid<_Tp, _Alloc>::size_type size)
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator Grid<_Tp, _Alloc>::const_iterator::operator-(typename Grid<_Tp, _Alloc>::size_type size) const
{

}

template<typename _Tp, typename _Alloc>
typename std::iterator<std::random_access_iterator_tag, typename Grid<_Tp, _Alloc>::value_type>::difference_type Grid<_Tp, _Alloc>::const_iterator::operator-(const typename Grid<_Tp, _Alloc>::const_iterator &another) const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator::const_reference Grid<_Tp, _Alloc>::const_iterator::operator*() const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator::const_point Grid<_Tp, _Alloc>::const_iterator::operator->() const
{

}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_iterator::const_reference Grid<_Tp, _Alloc>::const_iterator::operator[](typename Grid<_Tp, _Alloc>::size_type offset) const
{

}

#endif // GRID_HPP

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
		throw std::out_of_range("out of range");
	else
		return m_ptr(column);
}

template<typename _Tp, typename _Alloc>
typename Grid<_Tp, _Alloc>::const_reference Grid<_Tp, _Alloc>::_Grid_Row::operator[](typename Grid<_Tp, _Alloc>::size_type column) const
{

}
