#pragma once

#include <stdexcept>

namespace PrimMatrix
{
	class DMatrix_Exception : public std::runtime_error
	{
	public:
		explicit DMatrix_Exception(const char* msg) :
			std::runtime_error{ msg } {}
	};

	class DMatrix_InvalidInitializerSize : public DMatrix_Exception
	{
	public:
		explicit DMatrix_InvalidInitializerSize(const size_t initializer_size, const size_t matrix_size) :
			DMatrix_Exception{ "Passed initializer used to initialize the matrix has different size than the matrix" },
			initializer_size_{ initializer_size },
			matrix_size_{ matrix_size }
		{

		}

		size_t initializer_size() const { return initializer_size_; }
		size_t matrix_size() const { return matrix_size_; }


	private:
		const size_t initializer_size_, matrix_size_;
	};

	class DMatrix_IndexOutOfBounds : public DMatrix_Exception
	{
	public:
		explicit DMatrix_IndexOutOfBounds(const size_t index, const size_t matrix_size) :
			DMatrix_Exception{ "Index is out of bounds" },
			index_{ index },
			matrix_size_{ matrix_size }
		{

		}

		size_t index() const { return index_; }
		size_t matrix_size() const { return matrix_size_; }

	private:
		const size_t index_, matrix_size_;
	};

	class DMatrix_RowColOutOfBounds : public DMatrix_Exception
	{
	public:
		explicit DMatrix_RowColOutOfBounds(const size_t rows, const size_t columns, const size_t matrixRows, const size_t matrixColumns) :
			DMatrix_Exception{ "Row or column is out of bounds" },
			rows_{ rows },
			columns_{ columns },
			matrix_rows_{ matrixRows },
			matrix_columns_{ matrixColumns }
		{

		}

		size_t rows() const { return rows_; }
		size_t columns() const { return columns_; }
		size_t matrix_rows() const { return matrix_rows_; }
		size_t matrix_columns() const { return matrix_columns_; }

	private:
		const size_t rows_, columns_, matrix_rows_, matrix_columns_;
	};

	class DMatrix_OperationMatrixMismatch : public DMatrix_Exception
	{
	public:
		enum class EOperation
		{
			addition,
			subtraction,
			multiplication
		};

		explicit DMatrix_OperationMatrixMismatch(const EOperation operation, const size_t lhs_rows, const size_t lhs_columns, const size_t rhs_rows, const size_t rhs_columns) :
			DMatrix_Exception{ "Operation cannot be performed on these matrices" },
			operation_{ operation },
			lhs_rows_{ lhs_rows },
			lhs_columns_{ lhs_columns },
			rhs_rows_{ rhs_rows },
			rhs_columns_{ rhs_columns }
		{

		}

		EOperation operation() const { return operation_; };
		size_t lhs_rows() const { return lhs_rows_; }
		size_t lhs_columns() const { return lhs_columns_; }
		size_t rhs_rows() const { return rhs_rows_; }
		size_t rhs_columns() const { return rhs_columns_; }


	private:
		const EOperation operation_;
		const size_t lhs_rows_, lhs_columns_, rhs_rows_, rhs_columns_;
	};
}