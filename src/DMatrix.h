#pragma once

#include <vector>
#include <exception>
#include <ostream>

namespace PrimMatrix
{
	template <class T>
	class DMatrix
	{

	public:
		enum class EOrientation 
		{ 
			Horizontal, 
			Vertical 
		};

	public:
		/* CONSTRUCTION || DESTRUCTION */
		explicit DMatrix(const int rowCount, const int columnCount) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount);
		}

		explicit DMatrix(const int rowCount, const int columnCount, const T& init) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount, init);
		}

		explicit DMatrix(const int rowCount, const int columnCount, const std::vector<T>& arr) :
			_rowCount{ rowCount },
			_columncount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);
			
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const int matrixSize = rowCount * columnCount;
			_data.resize(matrixSize);
			_data = arr;
		}

		explicit DMatrix(const std::vector<T>& arr, const EOrientation orientation) :
			_rowCount{ orientation == EOrientation::Horizontal ? arr.size() : 1 },
			_columnCount{ orientation == EOrientation::Vertical ? arr.size() : 1 }
		{
			if (arr.size() == 0)
			{
				throw DMatrix_ArrayIsEmpty{};
			}

			const int matrixSize = rowCount * columnCount;
			_data.resize(matrixSize);
			_data = arr;

			assert(_data.capacity() == (_columnCount *_rowCount), "_data capacity is different than max size");
		}

		DMatrix(const DMatrix&) = default;
		DMatrix(DMatrix&&) noexcept = default;

		DMatrix& operator=(const DMatrix&) = default;
		DMatrix& operator=(DMatrix&&) noexcept = default;

		/* ACCESSORS */
		int rows() const noexcept { return _rowCount; }
		int columns() const noexcept { return _columnCount; }
		int size() const noexcept { return _data.size(); }

		T& operator()(int row, int column)
		{
			if (row >= _rowCount ||
				column >= _columnCount)
			{
				throw DMatrix_OutOfBounds{};
			}

			return _data[rowColToIndex(row, column)];
		}

		const T& operator()(int row, int column) const
		{
			if (row >= _rowCount ||
				column >= _columnCount)
			{
				throw DMatrix_OutOfBounds{};
			}

			return _data[rowColToIndex(row, column)];
		}


	private:
		void AssertMatrixSize(const int rowCount, const int columnCount)
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

		size_t rowColToIndex(int row, int column) const
		{
			return row * _columnCount + column;
		}

		int _rowCount, _columnCount;
		std::vector<T> _data;

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
	class DMatrix_Exception : public std::exception
	{
	public:
		DMatrix_Exception(const char* msg) :
			std::exception{msg} {}
	};

	class DMatrix_WrongRowCountException : public DMatrix_Exception
	{
	public:
		DMatrix_WrongRowCountException() :
			DMatrix_Exception{"Row count needs to be equal or greater than 1"} {}
	};

	class DMatrix_WrongColumnCountException : public DMatrix_Exception
	{
	public:
		DMatrix_WrongColumnCountException() :
			DMatrix_Exception{ "Column count needs to be equal or greater than 1" } {}
	};

	class DMatrix_ArrayIsEmpty : public DMatrix_Exception
	{
	public:
		DMatrix_ArrayIsEmpty() :
			DMatrix_Exception{"Passed array used to initialize the matrix is empty"} {}
	};

	class DMatrix_OutOfBounds : public DMatrix_Exception
	{
	public:
		DMatrix_OutOfBounds() :
			DMatrix_Exception{ "Index, row or column is out of bounds" } {}
	};
}

