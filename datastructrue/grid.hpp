#ifndef GRID_H
#define GRID_H
#include <iterator>
#include <initializer_list>

// Declaration
template<typename T>
class Grid
{
public:
	typedef class __Grid_Iterator : public std::iterator<std::input_iterator_tag, T>
	{
	public:
		__Grid_Iterator(T* p);

		__Grid_Iterator &operator =(const __Grid_Iterator &iter);

		bool operator !=(const __Grid_Iterator &iter) const;

		bool operator ==(const __Grid_Iterator &iter) const;

		__Grid_Iterator &operator ++();

		__Grid_Iterator operator ++(int);

		T &operator *();

	private:
		T* _ptr;
	} iterator;

	Grid();
	Grid(size_t rows, size_t cols);
	Grid(size_t rows ,size_t cols, const T &value);
	Grid(std::initializer_list<std::initializer_list<T>> values);
	Grid(const Grid<T> &other);
	~Grid();

	Grid<T> &operator =(const Grid<T> &other);

	void clear();

	T get(size_t row, size_t col) const;
	T *operator [](size_t index);

	size_t rows() const;
	size_t columns() const;
	size_t size() const;
	size_t count(const T &value) const;

	iterator begin();
	iterator end();

	void resize(size_t rows, size_t cols);
	void resize(size_t rows, size_t cols, const T &value);
	void resize(std::initializer_list<std::initializer_list<T>> values);

	bool operator ==(const Grid<T> &other) const;
	bool operator !=(const Grid<T> &other) const;

	size_t replace(const T &oldValue, const T &newValue);
	void setRow(size_t index, const T &value);
	void setColumn(size_t index, const T &value);

private:
	T *m_grid;
	size_t m_rows, m_columns;
};

// Definition
template<typename T>
Grid<T>::Grid() :
	m_grid(nullptr) ,m_columns(0), m_rows(0)
{

}

template<typename T>
Grid<T>::Grid(size_t rows, size_t cols) :
	m_rows(rows), m_columns(cols)
{
	m_grid = new T[rows * cols];
	for(size_t i = 0; i < rows * cols; i++)
		m_grid[i] = T();
}

template<typename T>
Grid<T>::Grid(size_t rows, size_t cols, const T &value) :
	m_rows(rows), m_columns(cols)
{
	m_grid = new T[rows * cols];
	for(size_t i = 0; i < rows * cols; i++)
		m_grid[i] = value;
}

template<typename T>
Grid<T>::Grid(std::initializer_list<std::initializer_list<T>> values)
{
	m_rows = values.size();
	m_columns = 0;
	for(auto i : values)
	{
		if(i.size() > m_columns)
			m_columns = i.size();
	}

	m_grid = new T[m_rows * m_columns];

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

template<typename T>
Grid<T>::Grid(const Grid<T> &other)
{
	m_rows = other.rows();
	m_columns = other.columns();

	m_grid = new T[m_rows * m_columns];

	for(size_t i = 0; i < m_rows; i++)
		for(size_t j = 0; j < m_columns; j++)
			m_grid[i][j] = other.get(i, j);
}

template<typename T>
Grid<T>::~Grid()
{
	clear();
}

template<typename T>
Grid<T> &Grid<T>::operator =(const Grid<T> &other)
{
	clear();

	m_rows = other.rows();
	m_columns = other.columns();

	m_grid = new T[m_rows * m_columns];

	for(size_t i = 0; i < m_rows; i++)
		for(size_t j = 0; j < m_columns; j++)
			m_grid[i][j] = other.get(i, j);
}

template<typename T>
void Grid<T>::clear()
{
	if(m_grid)
		delete[] m_grid;
	m_grid = nullptr;
	m_rows = 0;
	m_columns = 0;
}

template<typename T>
T Grid<T>::get(size_t row, size_t col) const
{
	if(row < m_rows && col < m_columns)
		return m_grid[row * m_columns + col];
	else
		return T();
};

template<typename T>
T *Grid<T>::operator[](size_t index)
{
	if(index < m_rows)
		return m_grid + index * m_columns;
	else
		return nullptr;
}

template<typename T>
size_t Grid<T>::rows() const
{
	return m_rows;
}

template<typename T>
size_t Grid<T>::columns() const
{
	return m_columns;
}

template<typename T>
size_t Grid<T>::size() const
{
	return m_columns * m_rows;
}

template<typename T>
size_t Grid<T>::count(const T &value) const
{
	size_t number = 0;
	for(size_t i = 0; i < m_rows * m_columns; i++)
		if(m_grid[i] == value)
			number++;

	return number;
}

template<typename T>
typename Grid<T>::iterator Grid<T>::begin()
{
	return iterator(m_grid);
}

template<typename T>
typename Grid<T>::iterator Grid<T>::end()
{
	return iterator(m_grid + size());
}

template<typename T>
void Grid<T>::resize(size_t rows, size_t cols)
{
	clear();

	m_rows = rows;
	m_columns = cols;

	m_grid = new T[rows * cols];
	for(size_t i = 0; i < rows * cols; i++)
		m_grid[i] = T();
}

template<typename T>
void Grid<T>::resize(size_t rows, size_t cols, const T &value)
{
	clear();

	m_rows = rows;
	m_columns = cols;

	m_grid = new T[rows * cols];
	for(size_t i = 0; i < rows * cols; i++)
		m_grid[i] = value;
}

template<typename T>
void Grid<T>::resize(std::initializer_list<std::initializer_list<T> > values)
{
	clear();

	m_rows = values.size();
	m_columns = 0;
	for(auto i : values)
	{
		if((*i).size() > m_columns)
			m_columns = (*i).size();
	}

	m_grid = new T[m_rows * m_columns];

	int row = 0;
	for(auto i : values)
	{
		int column = 0;
		for(auto j : i)
		{
			m_grid[row * m_columns + column] = *j;
			column++;
		}
		row++;
	}
}

template<typename T>
bool Grid<T>::operator ==(const Grid<T> &other) const
{
	if(m_rows != other.rows())
		return false;

	if(m_columns != other.columns())
		return false;

	for(int i = 0; i < m_rows; i++)
		for(int j = 0; j < m_columns; j++)
			if(m_grid[i * m_columns + j] != other.get(i, j))
				return false;

	return true;
}

template<typename T>
bool Grid<T>::operator !=(const Grid<T> &other) const
{
	return ! operator==(other);
}

template<typename T>
size_t Grid<T>::replace(const T &oldValue, const T &newValue)
{
	size_t number = 0;
	for(int i = 0; i < m_rows * m_columns; i++)
		if(m_grid[i] == oldValue)
		{
			m_grid[i] = newValue;
			number ++;
		}

	return number;
}

template<typename T>
void Grid<T>::setRow(size_t index, const T &value)
{
	if(index < m_rows)
		for(int i = index * m_columns; i < (index + 1) * m_columns; i++)
			m_grid[i] = value;
}

template<typename T>
void Grid<T>::setColumn(size_t index, const T &value)
{
	if(index < m_columns)
		for(int i = index; i < m_rows * m_columns; i += m_columns)
			m_grid[i] = value;
}

template<typename T>
Grid<T>::__Grid_Iterator::__Grid_Iterator(T *p)
{
	_ptr = p;
}

template<typename T>
typename Grid<T>::__Grid_Iterator &Grid<T>::__Grid_Iterator::operator =(const typename Grid<T>::__Grid_Iterator &iter)
{
	_ptr = iter._ptr;
	return *this;
}

template<typename T>
bool Grid<T>::__Grid_Iterator::operator !=(const typename Grid::__Grid_Iterator &iter) const
{
	return _ptr != iter._ptr;
}

template<typename T>
bool Grid<T>::__Grid_Iterator::operator ==(const typename Grid::__Grid_Iterator &iter) const
{
	return _ptr == iter._ptr;
}

template<typename T>
typename Grid<T>::__Grid_Iterator &Grid<T>::__Grid_Iterator::operator ++()
{
	_ptr++;
	return *this;
}

template<typename T>
typename Grid<T>::__Grid_Iterator Grid<T>::__Grid_Iterator::operator ++(int)
{
	__Grid_Iterator tmp= *this;
	_ptr++;
	return tmp;
}

template<typename T>
T &Grid<T>::__Grid_Iterator::operator *()
{
	return *_ptr;
}

#endif // GRID_H
