#ifndef GRID_H
#define GRID_H
#include <iterator>
#include <initializer_list>
#include <exception>
#include <algorithm>

// Declaration
template<typename T>
class Grid
{
public:
	typedef class __Grid_Const_Iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		__Grid_Const_Iterator(T *const p);

		__Grid_Const_Iterator &operator =(const __Grid_Const_Iterator &iter);

		bool operator !=(const __Grid_Const_Iterator &iter) const;

		bool operator ==(const __Grid_Const_Iterator &iter) const;

		__Grid_Const_Iterator &operator ++();

		__Grid_Const_Iterator operator ++(int);

		const T &operator *() const;

	private:
		T *const _ptr;

	} const_iterator;

	typedef class __Grid_Iterator : public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		__Grid_Iterator(T *p);

		__Grid_Iterator &operator =(const __Grid_Iterator &iter);

		bool operator !=(const __Grid_Iterator &iter) const;

		bool operator ==(const __Grid_Iterator &iter) const;

		__Grid_Iterator &operator ++();

		__Grid_Iterator operator ++(int);

		T &operator *();

		operator const_iterator() const;

	private:
		T *_ptr;
	} iterator;

	Grid();
	Grid(size_t rows, size_t cols);
	Grid(size_t rows ,size_t cols, const T &value);
	Grid(std::initializer_list<std::initializer_list<T>> values);
	Grid(const Grid<T> &other);
	~Grid();

	Grid<T> &operator =(const Grid<T> &other);

	void clear();

	T at(size_t row, size_t col) const;
	T *operator [](size_t index);
	const T *operator [](size_t index) const;

	T &front();
	const T &front() const;

	T &back();
	const T &back() const;

	size_t rows() const;
	size_t columns() const;
	size_t size() const;
	size_t count(const T &value) const;
	bool empty() const;

	iterator begin();
	iterator end();

	const_iterator cbegin() const;
	const_iterator cend() const;

	void resize(size_t rows, size_t cols);
	void resize(size_t rows, size_t cols, const T &value);
	void resize(std::initializer_list<std::initializer_list<T>> values);

	bool operator ==(const Grid<T> &other) const;
	bool operator !=(const Grid<T> &other) const;

	size_t replace(const T &oldValue, const T &newValue);
	size_t replace(const T &oldValue, const T &newValue, size_t row1, size_t col1, size_t row2, size_t col2);
	size_t replace(const T &oldValue, const T &newValue, iterator begin, iterator end);
	void fillRow(size_t index, const T &value);
	void fillColumn(size_t index, const T &value);
	void fill(const T &value);
	void fill(const T &value, size_t row1, size_t col1, size_t row2, size_t col2);
	void fill(const T &value, iterator begin, iterator end);

	void reverse(bool top_bottom = true, bool left_right = true);
	void swap(Grid &other);

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
			m_grid[i][j] = other.at(i, j);
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
			m_grid[i][j] = other.at(i, j);
}

template<typename T>
void Grid<T>::clear()
{
	if(m_grid != nullptr)
		delete[] m_grid;
	m_grid = nullptr;
	m_rows = 0;
	m_columns = 0;
}

template<typename T>
T Grid<T>::at(size_t row, size_t col) const
{
	if(row < m_rows && col < m_columns)
		return m_grid[row * m_columns + col];
	else
		throw std::out_of_range(std::string("Index out of range : (") + std::to_string(row) + std::string(", ") + std::to_string(col) +
								std::string(")/(") + std::to_string(m_rows - 1) + std::string(", ") + std::to_string(m_columns - 1) + std::string(")."));
};

template<typename T>
T *Grid<T>::operator[](size_t index)
{
	if(index < m_rows)
		return m_grid + index * m_columns;
	else
		throw std::out_of_range(std::string("Index out of range : (") + std::to_string(index) + std::string(", ?)/(") + std::to_string(m_rows - 1) + std::string(", ?)."));
}

template<typename T>
const T *Grid<T>::operator [](size_t index) const
{
	if(index < m_rows)
		return m_grid + index * m_columns;
	else
		throw std::out_of_range(std::string("Index out of range : (") + std::to_string(index) + std::string(", ?)/(") + std::to_string(m_rows - 1) + std::string(", ?)."));
}

template<typename T>
T &Grid<T>::front()
{
	if(m_grid == nullptr)
		throw std::out_of_range(std::string("Index out of range : (0, 0)/(null, null)."));
	else
		return m_grid[0];
}

template<typename T>
const T &Grid<T>::front() const
{
	if(m_grid == nullptr)
		throw std::out_of_range(std::string("Index out of range : (0, 0)/(null, null)"));
	else
		return m_grid[0];
}

template<typename T>
T &Grid<T>::back()
{
	if(m_grid == nullptr)
		throw std::out_of_range(std::string("Index out of range : (0, 0)/(null, null)"));
	else
		return m_grid[m_rows * m_columns - 1];
}

template<typename T>
const T &Grid<T>::back() const
{
	if(m_grid == nullptr)
		throw std::out_of_range(std::string("Index out of range : (0, 0)/(null, null)"));
	else
		return m_grid[m_rows * m_columns - 1];
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
bool Grid<T>::empty() const
{
	return m_grid == nullptr;
}

template<typename T>
typename Grid<T>::iterator Grid<T>::begin()
{
	return iterator(m_grid);
}

template<typename T>
typename Grid<T>::iterator Grid<T>::end()
{
	return iterator(m_grid + m_rows * m_columns);
}

template<typename T>
typename Grid<T>::const_iterator Grid<T>::cbegin() const
{
	return const_iterator(m_grid);
}

template<typename T>
typename Grid<T>::const_iterator Grid<T>::cend() const
{
	return const_iterator(m_grid + m_rows * m_columns);
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
			if(m_grid[i * m_columns + j] != other.at(i, j))
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
size_t Grid<T>::replace(const T &oldValue, const T &newValue, size_t row1, size_t col1, size_t row2, size_t col2)
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

	if(row2 >= m_rows || col2 >= m_columns)
		throw std::out_of_range(std::string("Index out of range : (") + std::to_string(row2) + std::string(", ") + std::to_string(col2) +
								std::string(")/(") + std::to_string(m_rows - 1) + std::string(", ") + std::to_string(m_columns - 1) + std::string(")."));

	size_t number = 0;
	for(size_t i = row1; i <= row2; i++)
		for(size_t j = col1; j < col2; j++)
			if(m_grid[i * m_columns + j] == oldValue)
			{
				m_grid[i * m_columns + j] = newValue;
				number++;
			}

	return number;
}

template<typename T>
size_t Grid<T>::replace(const T &oldValue, const T &newValue, typename Grid<T>::iterator begin, typename Grid<T>::iterator end)
{
	size_t number = 0;
	for(;begin != end; begin++)
		if(*begin == oldValue)
		{
			*begin = newValue;
			number++;
		}

	return number;
}

template<typename T>
void Grid<T>::fillRow(size_t index, const T &value)
{
	if(index < m_rows)
		for(int i = index * m_columns; i < (index + 1) * m_columns; i++)
			m_grid[i] = value;
}

template<typename T>
void Grid<T>::fillColumn(size_t index, const T &value)
{
	if(index < m_columns)
		for(int i = index; i < m_rows * m_columns; i += m_columns)
			m_grid[i] = value;
}

template<typename T>
void Grid<T>::fill(const T &value)
{
	for(int i = 0; i < m_rows * m_columns; i++)
		m_grid[i] = value;
}

template<typename T>
void Grid<T>::fill(const T &value, size_t row1, size_t col1, size_t row2, size_t col2)
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

	if(row2 >= m_rows || col2 >= m_columns)
		throw std::out_of_range(std::string("Index out of range : (") + std::to_string(row2) + std::string(", ") + std::to_string(col2) +
								std::string(")/(") + std::to_string(m_rows - 1) + std::string(", ") + std::to_string(m_columns - 1) + std::string(")."));

	for(size_t i = row1; i <= row2; i++)
		for(size_t j = col1; j < col2; j++)
			m_grid[i * m_columns + j] = value;
}

template<typename T>
void Grid<T>::fill(const T &value, typename Grid<T>::iterator begin, typename Grid<T>::iterator end)
{
	for(;begin != end; begin++)
		*begin = value;
}

template<typename T>
void Grid<T>::reverse(bool top_bottom, bool left_right)
{
	if(m_grid == nullptr)
		return;

	if(!top_bottom && !left_right)
		return;

	if(top_bottom && !left_right)
	{
		T temp;
		for(int i = 0; i < m_rows / 2; i++)
			for(int j = 0; j < m_columns; j++)
			{
				temp = m_grid[i * m_columns + j];
				m_grid[i * m_columns + j] = m_grid[(m_rows - 1 - i) * m_columns + j];
				m_grid[(m_rows - 1 - i) * m_columns + j] = temp;
			}

		return;
	}

	if(!top_bottom && left_right)
	{
		for(int i = 0; i < m_rows; i++)
			std::reverse(m_grid + i * m_columns, m_grid + (i + 1) * m_columns);

		return;
	}

	if(top_bottom && left_right)
	{
		std::reverse(m_grid, m_grid + m_rows * m_columns);
		return;
	}
}

template<typename T>
void Grid<T>::swap(Grid<T> &other)
{
	T *temp = m_grid;
	m_grid = other.m_grid;
	other.m_grid = temp;

	m_rows ^= other.m_rows;
	other.m_rows ^= m_rows;
	m_rows ^= other.m_rows;

	m_columns ^= other.m_columns;
	other.m_columns ^= m_columns;
	m_columns ^= other.m_columns;
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
	__Grid_Iterator tmp = *this;
	_ptr++;
	return tmp;
}

template<typename T>
T &Grid<T>::__Grid_Iterator::operator *()
{
	return *_ptr;
}

template<typename T>
Grid<T>::__Grid_Const_Iterator::__Grid_Const_Iterator(T *const p)
{
	_ptr = p;
}

template<typename T>
typename Grid<T>::__Grid_Const_Iterator &Grid<T>::__Grid_Const_Iterator::operator =(const typename Grid<T>::__Grid_Const_Iterator &iter)
{
	_ptr = iter._ptr;
	return *this;
}

template<typename T>
bool Grid<T>::__Grid_Const_Iterator::operator !=(const typename Grid<T>::__Grid_Const_Iterator &iter) const
{
	return _ptr != iter._ptr;
}

template<typename T>
bool Grid<T>::__Grid_Const_Iterator::operator ==(const typename Grid<T>::__Grid_Const_Iterator &iter) const
{
	return _ptr == iter._ptr;
}

template<typename T>
typename Grid<T>::__Grid_Const_Iterator &Grid<T>::__Grid_Const_Iterator::operator ++()
{
	_ptr++;
	return *this;
}

template<typename T>
typename Grid<T>::__Grid_Const_Iterator Grid<T>::__Grid_Const_Iterator::operator ++(int)
{
	__Grid_Const_Iterator tmp = *this;
	_ptr++;
	return tmp;
}

template<typename T>
const T &Grid<T>::__Grid_Const_Iterator::operator *() const
{
	return *_ptr;
}

template<typename T>
Grid<T>::__Grid_Iterator::operator Grid<T>::const_iterator() const
{
	Grid<T>::const_iterator citr(_ptr);
	return citr;
}

#endif // GRID_H
