#pragma once

#include <stdexcept>

namespace PrimMatrix
{
	// Leaving it here for simplicity
	struct Splice
	{
		size_t row_begin;
		size_t column_begin;
		size_t row_count;
		size_t column_count;
	};

	inline bool operator==(const Splice& lhs, const Splice& rhs)
	{
		return lhs.row_begin == rhs.row_begin &&
			lhs.column_begin == rhs.column_begin &&
			lhs.row_count == rhs.row_count &&
			lhs.column_count == rhs.column_count;
	}

	class Matrix_Exception : public std::runtime_error
	{
	public:
		explicit Matrix_Exception(const char* msg) :
			std::runtime_error{ msg } {}
	};

	class Matrix_InvalidInitializerSize : public Matrix_Exception
	{
	public:
		explicit Matrix_InvalidInitializerSize(const size_t initializer_size, const size_t matrix_size) :
			Matrix_Exception{ "Passed initializer used to initialize the matrix has different size than the matrix" },
			initializer_size_{ initializer_size },
			matrix_size_{ matrix_size }
		{

		}

		size_t initializer_size() const noexcept { return initializer_size_; }
		size_t matrix_size() const noexcept { return matrix_size_; }


	private:
		const size_t initializer_size_, matrix_size_;
	};

	class Matrix_IndexOutOfBounds : public Matrix_Exception
	{
	public:
		explicit Matrix_IndexOutOfBounds(const size_t index, const size_t matrix_size) :
			Matrix_Exception{ "Index is out of bounds" },
			index_{ index },
			matrix_size_{ matrix_size }
		{

		}

		size_t index() const noexcept { return index_; }
		size_t matrix_size() const noexcept { return matrix_size_; }

	private:
		const size_t index_, matrix_size_;
	};

	class Matrix_RowColOutOfBounds : public Matrix_Exception
	{
	public:
		explicit Matrix_RowColOutOfBounds(const size_t rows, const size_t columns, const size_t matrixRows, const size_t matrixColumns) :
			Matrix_Exception{ "Row or column is out of bounds" },
			rows_{ rows },
			columns_{ columns },
			matrix_rows_{ matrixRows },
			matrix_columns_{ matrixColumns }
		{

		}

		size_t rows() const noexcept { return rows_; }
		size_t columns() const noexcept { return columns_; }
		size_t matrix_rows() const noexcept { return matrix_rows_; }
		size_t matrix_columns() const noexcept { return matrix_columns_; }

	private:
		const size_t rows_, columns_, matrix_rows_, matrix_columns_;
	};

	class Matrix_OperationMatrixMismatch : public Matrix_Exception
	{
	public:
		enum class EOperation
		{
			addition,
			subtraction,
			multiplication
		};

		explicit Matrix_OperationMatrixMismatch(const EOperation operation, const size_t lhs_rows, const size_t lhs_columns, const size_t rhs_rows, const size_t rhs_columns) :
			Matrix_Exception{ "Operation cannot be performed on these matrices, size mismatch" },
			operation_{ operation },
			lhs_rows_{ lhs_rows },
			lhs_columns_{ lhs_columns },
			rhs_rows_{ rhs_rows },
			rhs_columns_{ rhs_columns }
		{

		}

		EOperation operation() const noexcept { return operation_; };
		size_t lhs_rows() const noexcept { return lhs_rows_; }
		size_t lhs_columns() const noexcept { return lhs_columns_; }
		size_t rhs_rows() const noexcept { return rhs_rows_; }
		size_t rhs_columns() const noexcept { return rhs_columns_; }

	private:
		const EOperation operation_;
		const size_t lhs_rows_, lhs_columns_, rhs_rows_, rhs_columns_;
	};

	class Matrix_SpliceOutOfBounds : public Matrix_Exception
	{
	public:
		explicit Matrix_SpliceOutOfBounds(const Splice& splice, const size_t matrix_rows, const size_t matrix_columns) :
			Matrix_Exception{ "Splice is out of bounds" },
			splice_{ splice },
			matrix_rows_{ matrix_rows },
			matrix_columns_{ matrix_columns }
		{

		}

		const Splice& splice() const noexcept { return splice_; }
		size_t matrix_rows() const noexcept { return matrix_rows_; }
		size_t matrix_columns() const noexcept { return matrix_columns_; }

	private:
		const Splice splice_;
		const size_t matrix_rows_, matrix_columns_;
	};
}