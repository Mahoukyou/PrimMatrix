#pragma once

#include <vector>

#include "Matrix_Exception.h"

namespace PrimMatrix
{
	namespace
	{
		bool will_overflow(const size_t a, const size_t b)
		{
			// todo
			return true;
		}
	}

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
		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;

		enum class EOrientation 
		{ 
			horizontal, 
			vertical 
		};

		/* CONSTRUCTION || DESTRUCTION */
		explicit DMatrix(const size_type row_count, const size_type column_count) :
			rows_{ row_count },
			columns_{ column_count },
			data_(rows_ * columns_)
		{ 
			
		}

		explicit DMatrix(const size_type row_count, const size_type column_count, const value_type& initial_value) :
			rows_{ row_count },
			columns_{ column_count }, 
			data_(rows_ * columns_, initial_value)
		{

		}

		explicit DMatrix(const size_type row_count, const size_type column_count, const std::vector<value_type>& arr) :
			rows_{ row_count },
			columns_{ column_count }
		{
			const size_type size = rows() * columns();
			if(arr.size() != size)
			{
				throw Matrix_InvalidInitializerSize{ arr.size(), size };
			}

			data_ = arr;
		}

		explicit DMatrix(const size_type row_count, const size_type column_count, std::initializer_list<value_type> il) : 
			rows_{ row_count },
			columns_{ column_count }
		{
			const size_type size = rows() * columns();
			if (il.size() != size)
			{
				throw Matrix_InvalidInitializerSize{ il.size(), size };
			}

			data_ = std::move(il);
		}

		explicit DMatrix(const std::vector<value_type>& arr, const EOrientation orientation) :
			rows_{ orientation == EOrientation::vertical ? arr.size() : 1 },
			columns_{ orientation == EOrientation::horizontal ? arr.size() : 1 },
			data_{ arr }
		{
		}

		virtual ~DMatrix() = default;

		DMatrix(const DMatrix&) = default;
		DMatrix(DMatrix&& rhs) noexcept :
			rows_{ rhs.rows_ },
			columns_{ rhs.columns_ },
			data_{ std::move(rhs.data_) }
		{
			rhs.rows_ = rhs.columns_ = 0;
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

			swap(lhs.rows_, rhs.rows_);
			swap(lhs.columns_, rhs.columns_);
			swap(lhs.data_, rhs.data_);
		}

		/* ACCESSORS */
		size_type rows() const noexcept { return rows_; }
		size_type columns() const noexcept { return columns_; }
		size_type size() const noexcept { return data_.size(); }

		pointer data() noexcept { return data_.data(); }
		const_pointer data() const noexcept { return data_.data(); }

		reference at(const size_type index)
		{
			if (index >= size())
			{
				throw Matrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		const_reference at(const size_type index) const
		{
			if (index >= size())
			{
				throw Matrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		reference at(const size_type row, const size_type column)
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw Matrix_RowColOutOfBounds{ row, column, rows(), columns() };
			}

			return data_[to_index(row, column)];
		}

		const_reference at(const size_type row, const size_type column) const
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw Matrix_RowColOutOfBounds{row, column, rows(), columns()};
			}

			return data_[to_index(row, column)];
		}

		iterator begin() noexcept
		{
			return data_.begin();
		}

		const_iterator begin() const noexcept
		{
			return cbegin();
		}

		iterator end() noexcept
		{
			return data_.end();
		}

		const_iterator end() const noexcept
		{
			return cend();
		}

		const_iterator cbegin() const noexcept
		{
			return data_.cbegin();
		}

		const_iterator cend() const noexcept
		{
			return data_.cend();
		}

		/* OPERATORS */
		reference operator[](const size_type index)
		{
			return data_[index];
		}

		const_reference operator[](const size_type index) const
		{
			return data_[index];
		}

		reference operator()(const size_type row, const size_type column)
		{
			return data_[to_index(row, column)];
		}

		const_reference operator()(const size_type row, const size_type column) const
		{
			return data_[to_index(row, column)];
		}

		DMatrix& operator+=(const DMatrix& rhs)
		{
			if (rows() != rhs.rows() ||
				columns() != rhs.columns())
			{
				throw Matrix_OperationMatrixMismatch {
					Matrix_OperationMatrixMismatch::EOperation::addition,
					rows(),
					columns(),
					rhs.rows(),
					rhs.columns() };
			}


			auto rhs_it = rhs.begin();
			for (auto& el : *this)
			{
				el += *rhs_it++;
			}

			return *this;
		}

		DMatrix& operator-=(const DMatrix& rhs)
		{
			if (rows() != rhs.rows() ||
				columns() != rhs.columns())
			{
				throw Matrix_OperationMatrixMismatch {
					Matrix_OperationMatrixMismatch::EOperation::subtraction,
					rows(),
					columns(),
					rhs.rows(),
					rhs.columns() };
			}


			auto rhs_it = rhs.begin();
			for (auto& el : *this)
			{
				el -= *rhs_it++;
			}

			return *this;
		}

		DMatrix& operator*=(const DMatrix& rhs)
		{
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
			DMatrix result_matrix{ columns_, rows_ };

			auto current_matrix_iterator = data_.begin();
			for (size_type result_column = 0; result_column < result_matrix.columns(); ++result_column)
			{
				for (size_type result_row = 0; result_row < result_matrix.rows(); ++result_row, ++current_matrix_iterator)
				{
					result_matrix(result_row, result_column) = *current_matrix_iterator;
				}
			}

			return result_matrix;
		}

		static DMatrix create_identity_matrix(const size_type size, const value_type& value = 1)
		{
			DMatrix identity_matrix{ size, size };
			
			for (size_type i = 0; i < size; ++i)
			{
				identity_matrix(i, i) = value;
			}

			return identity_matrix;
		}

	private:
		size_type to_index(const size_type row, const size_type column) const noexcept
		{
			return row * columns() + column;
		}

		size_type rows_, columns_;
		std::vector<value_type> data_;

	};

	template <class T>
	bool operator==(const DMatrix<T>& lhs, const DMatrix<T> rhs)
	{
		if (lhs.rows() != rhs.rows() ||
			lhs.columns() != rhs.columns())
		{
			return false;
		}

		auto lhs_it = lhs.begin();
		for (const auto& rhs_el : rhs)
		{
			if (*lhs_it++ != rhs_el)
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
		if (lhs.rows() != rhs.rows() ||
			lhs.columns() != rhs.columns())
		{
			throw Matrix_OperationMatrixMismatch {
				Matrix_OperationMatrixMismatch::EOperation::addition,
				lhs.rows(),
				lhs.columns(),
				rhs.rows(),
				rhs.columns() };
		}

		DMatrix<T> result_matrix(lhs.rows(), lhs.columns());
		
		auto lhs_it = lhs.begin();
		auto rhs_it = rhs.begin();
		for (auto& result_el : result_matrix)
		{
			result_el = *lhs_it++ + *rhs_it++;
		}

		return result_matrix;
	}

	template <class T>
	DMatrix<T> operator-(const DMatrix<T>& lhs, const DMatrix<T>& rhs)
	{
		if (lhs.rows() != rhs.rows() ||
			lhs.columns() != rhs.columns())
		{
			throw Matrix_OperationMatrixMismatch {
				Matrix_OperationMatrixMismatch::EOperation::subtraction,
				lhs.rows(),
				lhs.columns(),
				rhs.rows(),
				rhs.columns() };
		}

		DMatrix<T> result_matrix(lhs.rows(), lhs.columns());

		auto lhs_it = lhs.begin();
		auto rhs_it = rhs.begin();
		for (auto& result_el : result_matrix)
		{
			result_el = *lhs_it++ - *rhs_it++;
		}

		return result_matrix;
	}

	template <class T>
	DMatrix<T> operator*(const DMatrix<T>& lhs, const DMatrix<T>& rhs)
	{
		using size_type = typename DMatrix<T>::size_type;

		if (lhs.columns() != rhs.rows())
		{
			throw Matrix_OperationMatrixMismatch { 
				Matrix_OperationMatrixMismatch::EOperation::multiplication, 
				lhs.rows(), 
				lhs.columns(), 
				rhs.rows(), 
				rhs.columns() };
		}

		DMatrix<T> result_matrix{ lhs.rows(), rhs.columns() };

		const auto sum_len = lhs.columns();
		for (size_type result_row = 0; result_row < result_matrix.rows(); ++result_row)
		{
			for (size_type result_column = 0; result_column < result_matrix.columns(); ++result_column)
			{
				for (size_type sum_index = 0; sum_index < sum_len; ++sum_index)
				{
					result_matrix(result_row, result_column) += lhs(result_row, sum_index) * rhs(sum_index, result_column);
				}
			}
		}

		return result_matrix;
	}

	template <class T>
	DMatrix<T> operator*(const DMatrix<T>& lhs, const T& rhs)
	{
		DMatrix<T> result_matrix{ lhs.rows(), lhs.columns() };

		auto result_it = result_matrix.begin();
		for (const auto& lhs_value : lhs)
		{
			*result_it++ = lhs_value * rhs;
		}

		return result_matrix;
	}

	template <class T>
	DMatrix<T> operator*(const T& lhs, const DMatrix<T>& rhs)
	{
		return rhs * lhs;
	}	
}