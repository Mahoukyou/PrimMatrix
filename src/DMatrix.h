#pragma once

#include <vector>
#include <exception>
#include <ostream>
#include <assert.h>

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
		explicit DMatrix(const size_type rowCount, const size_type columnCount, const value_type& initialValue = value_type{}) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			_data.resize(rowCount * columnCount, initialValue);
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, const std::vector<value_type>& arr) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const size_type matrixSize = _rowCount * _columnCount;
			_data.resize(matrixSize);
			_data = arr;

			assert(_data.capacity() == matrixSize);
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, std::initializer_list<value_type> il) : 
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			if (il.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const size_type matrixSize = _rowCount * _columnCount;
			_data.resize(matrixSize);
			_data = il;


			assert(_data.capacity() == matrixSize);

		}

		explicit DMatrix(const std::vector<value_type>& arr, const EOrientation orientation) :
			_rowCount{ orientation == EOrientation::Vertical ? arr.size() : 1 },
			_columnCount{ orientation == EOrientation::Horizontal ? arr.size() : 1 }
		{
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const size_type matrixSize = _rowCount * _columnCount;
			_data.resize(matrixSize);
			_data = arr;

			assert(_data.capacity() == matrixSize);
		}

		/* COPY / MOVE OPERATIONS */
		DMatrix(const DMatrix&) = default;
		DMatrix(DMatrix&&) noexcept = default;

		DMatrix& operator=(const DMatrix&) = default;
		DMatrix& operator=(DMatrix&&) noexcept = default;

		/* ACCESSORS */
		size_type rows() const noexcept { return _rowCount; }
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

		/* OPERATIONS */
		DMatrix transpose() const
		{
			DMatrix resultMatrix{ _columnCount, _rowCount };

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

	private:
		//void swap(DMatrix& lhs, DMatrix& rhs) noexcept
		//{
		//	// Enable ADL
		//	using std::swap;

		//	swap(lhs._rowCount, rhs._rowCount);
		//	swap(lhs._columnCount, rhs._columnCount);
		//	swap(lhs._data, rhs._data);
		//}

		size_type rowColToIndex(const size_type row, const size_type column) const
		{
			return row * _columnCount + column;
		}

		size_type _rowCount, _columnCount;
		std::vector<value_type> _data;

	};

	template <class T>
	std::ostream& operator<<(std::ostream& os, const DMatrix<T>& matrix)
	{
		os << "Matrix size = " << matrix.size() << "\n";
		os << "Row count = " << matrix.rows() << " | Column count = " << matrix.columns() << "\n";
		
		for (size_t row = 0; row < matrix.rows(); ++row)
		{
			for (size_t column = 0; column < matrix.columns(); ++column)
			{
				os << "[" << matrix(row, column) << "] ";
			}

			os << "\n";
		}

		return os;
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
	DMatrix<T> operator*(const DMatrix<T> lhs, const DMatrix<T> rhs)
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
			*resultIt = lhsValue * rhs;
			++resultIt;
		}

		return resultMatrix;
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