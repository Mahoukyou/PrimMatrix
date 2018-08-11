#pragma once

#include <vector>
#include <exception>

namespace PrimMatrix
{
	template <class T>
	class DMatrix
	{

	public:
		/* CONSTRUCTION || DESTRUCTION */
		DMatrix(const int rowCount, const int columnCount) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount);
		}

		DMatrix(const int rowCount, const int columnCount, const T& init) :
			_rowCount{ rowCount },
			_columnCount{ columnCount }
		{
			AssertMatrixSize(rowCount, columnCount);

			_data.resize(rowCount * columnCount, init);
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
				// todo
				throw std::exception{};
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
			using std::swap;
			std::swap(lhs._rowCount, rhs._rowCount);
			std::swap(lhs._columnCount, rhs._columnCount);
			std::swap(lhs._size, rhs._size);
			std::swap(lhs._data, rhs._data);
		}

		size_t rowColToIndex(int row, int column)
		{
			return row * _columnCount + column;
		}

		int _rowCount, _columnCount;
		std::vector<T> _data;

	};

	/* DMatrix exceptions */
	class DMatrix_Exception : public std::exception
	{
	public:
		DMatrix_Exception(const char* msg) :
			std::exception(msg) {}
	};

	class DMatrix_WrongRowCountException : public DMatrix_Exception
	{
	public:
		DMatrix_WrongRowCountException() :
			DMatrix_Exception("Row count needs to be equal or greater than 1") {}
	};

	class DMatrix_WrongColumnCountException : public DMatrix_Exception
	{
	public:
		DMatrix_WrongColumnCountException() :
			DMatrix_Exception("Column count needs to be equal or greater than 1") {}
	};
}