#pragma once

#include <vector>
#include <exception>
#include <ostream>

namespace PrimMatrix
{
	// FORWARD DECLARATION
	class DMatrix_Exception;
	class DMatrix_ArrayIsEmpty;
	class DMatrix_IndexOutOfBounds;
	class DMatrix_RowColOutOfBounds;


	template <class T>
	class DMatrix
	{
	public:

		using value_type = T;
		using size_type = typename std::vector<T>::size_type;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		
		//tmp for now
		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;

		enum class EOrientation 
		{ 
			Horizontal, 
			Vertical 
		};

	public:
		/* CONSTRUCTION || DESTRUCTION */
		explicit DMatrix(const size_type rowCount, const size_type columnCount) :
			row_count_{ rowCount },
			_columnCount{ columnCount },
			_data(row_count_ * _columnCount)
		{ 
			
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, const value_type& initialValue) :
			row_count_{ rowCount },
			_columnCount{ columnCount }, 
			_data(row_count_ * _columnCount, initialValue)

		{

		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, const std::vector<value_type>& arr) :
			row_count_{ rowCount },
			_columnCount{ columnCount }
		{
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			_data = arr;
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, std::initializer_list<value_type> il) : 
			row_count_{ rowCount },
			_columnCount{ columnCount }
		{
			if (il.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			_data = std::move(il);
		}

		explicit DMatrix(const std::vector<value_type>& arr, const EOrientation orientation) :
			row_count_{ orientation == EOrientation::Vertical ? arr.size() : 1 },
			_columnCount{ orientation == EOrientation::Horizontal ? arr.size() : 1 }
		{
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			_data = arr;
		}

		virtual ~DMatrix() = default;

		DMatrix(const DMatrix&) = default;
		DMatrix(DMatrix&& rhs) noexcept :
			row_count_{ rhs.row_count_ },
			_columnCount{ rhs._columnCount },
			_data{ std::move(rhs._data) }
		{
			rhs.row_count_ = rhs._columnCount = 0;
		}

		/* COPY / MOVE OPERATIONS */
		DMatrix& operator=(const DMatrix&) = default;
		DMatrix& operator=(DMatrix&& rhs) noexcept
		{
			swap(*this, rhs);
			return *this;
		}
			
		friend void swap(DMatrix& lhs, DMatrix& rhs) noexcept
		{
			using std::swap;

			swap(lhs.row_count_, rhs.row_count_);
			swap(lhs._columnCount, rhs._columnCount);
			swap(lhs._data, rhs._data);
		}

		/* ACCESSORS */
		size_type rows() const noexcept { return row_count_; }
		size_type columns() const noexcept { return _columnCount; }
		size_type size() const noexcept { return _data.size(); }

		pointer data() noexcept { return _data.data(); }
		const_pointer data() const noexcept { return _data.data(); }

		reference at(const size_type index)
		{
			if (index >= size())
			{
				throw DMatrix_IndexOutOfBounds{ index, size() };
			}

			return _data[index];
		}

		const_reference at(const size_type index) const
		{
			if (index >= size())
			{
				throw DMatrix_IndexOutOfBounds{ index, size() };
			}

			return _data[index];
		}

		reference at(const size_type row, const size_type column)
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw DMatrix_RowColOutOfBounds{ row, column, rows(), columns() };
			}

			return _data[rowColToIndex(row, column)];
		}

		const_reference at(const size_type row, const size_type column) const
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw DMatrix_RowColOutOfBounds{row, column, rows(), columns()};
			}

			return _data[rowColToIndex(row, column)];
		}

		/* tmp ITERATORS -- vector ones for now */
		iterator begin() { return _data.begin(); }
		const_iterator begin() const { return _data.begin(); }
		iterator end() { return _data.end(); }
		const_iterator end() const { return _data.end(); }

		const_iterator cbegin() const { return _data.cbegin(); }
		const_iterator cend() const { return _data.cend(); }

		/* OPERATORS */
		reference operator[](const size_type index)
		{
			return _data[index];
		}

		const_reference operator[](const size_type index) const
		{
			return _data[index];
		}

		reference operator()(const size_type row, const size_type column)
		{
			return _data[rowColToIndex(row, column)];
		}

		const_reference operator()(const size_type row, const size_type column) const
		{
			return _data[rowColToIndex(row, column)];
		}

		DMatrix& operator+=(const DMatrix& rhs)
		{
			if (rows() != rhs.rows())
			{
				// todo
				throw 0;
			}

			if (columns() != rhs.columns())
			{
				// todo
				throw 0;
			}

			auto rhsIt = rhs.begin();
			for (auto& el : *this)
			{
				el += *rhsIt++;
			}

			return *this;
		}

		DMatrix& operator-=(const DMatrix& rhs)
		{
			if (rows() != rhs.rows())
			{
				// todo
				throw 0;
			}

			if (columns() != rhs.columns())
			{
				// todo
				throw 0;
			}

			auto rhsIt = rhs.begin();
			for (auto& el : *this)
			{
				el -= *rhsIt++;
			}

			return *this;
		}

		DMatrix& operator*=(const DMatrix& rhs)
		{
			//if (columns() != rhs.rows())
			//{
			//	// todo
			//	throw 0;
			//}

			// We need to allocate third matrice anyway, so we can do this, this way atm
			*this = *this * rhs;

			return *this;
		}

		DMatrix& operator*=(const T& rhs)
		{
			for (auto& el : *this)
			{
				el *= rhs;
			}

			return *this;
		}

		/* OPERATIONS */
		DMatrix transpose() const
		{
			DMatrix resultMatrix{ _columnCount, row_count_ };

			auto currentMatrixIterator = _data.begin();
			for (size_type resultColumn = 0; resultColumn < resultMatrix.columns(); ++resultColumn)
			{
				for (size_type resultRow = 0; resultRow < resultMatrix.rows(); ++resultRow, ++currentMatrixIterator)
				{
					resultMatrix(resultRow, resultColumn) = *currentMatrixIterator;
				}
			}

			return resultMatrix;
		}

		static DMatrix createIdentityMatrix(const size_type size, const value_type& value = 1)
		{
			DMatrix identityMatrix{ size, size };
			
			for (size_type i = 0; i < size; ++i)
			{
				identityMatrix(i, i) = value;
			}

			return identityMatrix;
		}

	private:
		size_type rowColToIndex(const size_type row, const size_type column) const
		{
			return row * _columnCount + column;
		}

		size_type row_count_, _columnCount;
		std::vector<value_type> _data;

	};

	template <class T>
	bool operator==(const DMatrix<T>& lhs, const DMatrix<T> rhs)
	{
		if (lhs.rows() != rhs.rows() ||
			lhs.columns() != rhs.columns())
		{
			return false;
		}

		auto lhsIt = lhs.begin();
		for (const auto& rhsEl : rhs)
		{
			if (*lhsIt++ != rhsEl)
			{
				return false;
			}
		}

		return true;
	}

	template<class T>
	bool operator!=(const DMatrix<T>& lhs, const DMatrix<T> rhs)
	{
		return !(lhs == rhs);
	}

	template <class T>
	DMatrix<T> operator+(const DMatrix<T>& lhs, const DMatrix<T>& rhs)
	{
		if (lhs.rows() != rhs.rows())
		{
			// todo
			throw 0;
		}

		if (lhs.columns() != rhs.columns())
		{
			// todo
			throw 0;
		}

		DMatrix<T> resultMatrix(lhs.rows(), lhs.columns());
		
		auto lhsIt = lhs.begin();
		auto rhsIt = rhs.begin();
		for (auto& resultEl : resultMatrix)
		{
			resultEl = *lhsIt++ + *rhsIt++;
		}

		return resultMatrix;
	}

	template <class T>
	DMatrix<T> operator-(const DMatrix<T>& lhs, const DMatrix<T>& rhs)
	{
		if (lhs.rows() != rhs.rows())
		{
			// todo
			throw 0;
		}

		if (lhs.columns() != rhs.columns())
		{
			// todo
			throw 0;
		}

		DMatrix<T> resultMatrix(lhs.rows(), lhs.columns());

		auto lhsIt = lhs.begin();
		auto rhsIt = rhs.begin();
		for (auto& resultEl : resultMatrix)
		{
			resultEl = *lhsIt++ - *rhsIt++;
		}

		return resultMatrix;
	}

	template <class T>
	DMatrix<T> operator*(const DMatrix<T>& lhs, const DMatrix<T>& rhs)
	{
		using size_type = typename DMatrix<T>::size_type;

		if (lhs.columns() != rhs.rows())
		{
			// todo
			throw 0;
		}

		DMatrix<T> resultMatrix{ lhs.rows(), rhs.columns() };

		const auto sumLen = lhs.columns();
		for (size_type resultRow = 0; resultRow < resultMatrix.rows(); ++resultRow)
		{
			for (size_type resultColumn = 0; resultColumn < resultMatrix.columns(); ++resultColumn)
			{
				for (size_type sumIndex = 0; sumIndex < sumLen; ++sumIndex)
				{
					resultMatrix(resultRow, resultColumn) += lhs(resultRow, sumIndex) * rhs(sumIndex, resultColumn);
				}
			}
		}

		return resultMatrix;
	}

	// Scalar multiplication
	template <class T>
	DMatrix<T> operator*(const DMatrix<T>& lhs, const T& rhs)
	{
		DMatrix<T> resultMatrix{ lhs.rows(), lhs.columns() };

		auto resultIt = resultMatrix.begin();
		for (const auto& lhsValue : lhs)
		{
			*resultIt++ = lhsValue * rhs;
		}

		return resultMatrix;
	}

	template <class T>
	DMatrix<T> operator*(const T& lhs, const DMatrix<T>& rhs)
	{
		return rhs * lhs;
	}

	/* DMatrix exceptions */
	class DMatrix_Exception : public std::runtime_error
	{
	public:
		explicit DMatrix_Exception(const char* msg) :
			std::runtime_error{ msg } {}
	};

	class DMatrix_ArrayIsEmpty : public DMatrix_Exception
	{
	public:
		explicit DMatrix_ArrayIsEmpty() :
			DMatrix_Exception{ "Passed array used to initialize the matrix is empty" } {}
	};

	class DMatrix_IndexOutOfBounds : public DMatrix_Exception
	{
	public:
		explicit DMatrix_IndexOutOfBounds(const size_t index, const size_t matrixSize) :
			DMatrix_Exception{ "Index is out of bounds" },
			_index{ index },
			_matrixSize{ matrixSize }
		{}

		size_t index() const { return _index; }
		size_t matrixSize() const { return _matrixSize; }

	private:
		const size_t _index, _matrixSize;
	};

	class DMatrix_RowColOutOfBounds : public DMatrix_Exception
	{
	public:
		explicit DMatrix_RowColOutOfBounds(const size_t rows, const size_t columns, const size_t matrixRows, const size_t matrixColumns) :
			DMatrix_Exception{ "Row or column is out of bounds" },
			_rows{ rows },
			_columns{ columns },
			_matrixRows{ matrixRows },
			_matrixColumn{ matrixColumns }

		{}

		size_t rows() const { return _rows; }
		size_t columns() const { return _columns; }
		size_t matrixRows() const { return _matrixRows; }
		size_t matrixColumns() const { return _matrixColumn; }

	private:
		const size_t _rows, _columns, _matrixRows, _matrixColumn;
	};
}