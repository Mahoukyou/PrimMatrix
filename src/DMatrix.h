#pragma once

#include <vector>
#include <exception>
#include <ostream>
#include <assert.h>

namespace PrimMatrix
{
	// FORWARD DECLARATION
	class DMatrix_Exception;
	class DMatrix_WrongRowCountException;
	class DMatrix_WrongColumnCountException;
	class DMatrix_ArrayIsEmpty;
	class DMatrix_OutOfBounds;


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

		enum class EOrientation 
		{ 
			Horizontal, 
			Vertical 
		};

	public:
		/* CONSTRUCTION || DESTRUCTION */
		explicit DMatrix(const size_type rowCount, const size_type columnCount) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount);
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, const value_type& init) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount, init);
		}

		explicit DMatrix(const size_type rowCount, const size_type columnCount, const std::vector<value_type>& arr) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(_rowCount, _columnCount);
			
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const size_type matrixSize = _rowCount * _columnCount;
			_data.resize(matrixSize);
			_data = arr;
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

			assert(_data.capacity() == (_columnCount *_rowCount));
		}

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

		/* OPERATORS */
		reference operator()(const size_type row, const size_type column)
		{
			if (row >= _rowCount ||
				column >= _columnCount)
			{
				throw DMatrix_OutOfBounds{};
			}

			return _data[rowColToIndex(row, column)];
		}

		const_reference operator()(const size_type row, const size_type column) const
		{
			if (row >= _rowCount ||
				column >= _columnCount)
			{
				throw DMatrix_OutOfBounds{};
			}

			return _data[rowColToIndex(row, column)];
		}



	private:
		void AssertMatrixSize(const size_type rowCount, const size_type columnCount) const
		{
			if (rowCount < 1)
			{
				throw DMatrix_WrongRowCountException{};
			}

			if (columnCount < 1)
			{
				throw DMatrix_WrongColumnCountException{};
			}
		}

		void swap(DMatrix& lhs, DMatrix& rhs) noexcept
		{
			// Enable ADL
			using std::swap;

			swap(lhs._rowCount, rhs._rowCount);
			swap(lhs._columnCount, rhs._columnCount);
			swap(lhs._size, rhs._size);
			swap(lhs._data, rhs._data);
		}

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

	/* DMatrix exceptions */
	class DMatrix_Exception : public std::runtime_error
	{
	public:
		explicit DMatrix_Exception(const char* msg) :
			std::runtime_error{ msg } {}
	};
	
	class DMatrix_WrongRowCountException : public DMatrix_Exception
	{
	public:
		explicit DMatrix_WrongRowCountException() :
			DMatrix_Exception{ "Row count needs to be equal or greater than 1" } {}
	};

	class DMatrix_WrongColumnCountException : public DMatrix_Exception
	{
	public:
		explicit DMatrix_WrongColumnCountException() :
			DMatrix_Exception{ "Column count needs to be equal or greater than 1" } {}
	};

	class DMatrix_ArrayIsEmpty : public DMatrix_Exception
	{
	public:
		explicit DMatrix_ArrayIsEmpty() :
			DMatrix_Exception{ "Passed array used to initialize the matrix is empty" } {}
	};

	class DMatrix_OutOfBounds : public DMatrix_Exception
	{
	public:
		explicit DMatrix_OutOfBounds() :
			DMatrix_Exception{ "Index, row or column is out of bounds" } {}
	};
}

