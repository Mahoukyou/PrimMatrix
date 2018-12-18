#pragma once

#include <array>
#include "Matrix_Exception.h"

// todo, use constructors instead of aggregate initialization?

namespace PrimMatrix
{
	template <class T, size_t rows_, size_t columns_>
	class SMatrix
	{
		static_assert(rows_ * columns_ != 0, "Empty matrices are not supported at the moment");

	public:
		using value_type = T;
		using size_type = size_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;

		// todo
		using iterator = pointer;
		using const_iterator = const_pointer;

		constexpr size_type rows() const noexcept { return rows_; };
		constexpr size_type columns() const noexcept { return columns_; };
		constexpr size_type size() const noexcept { return rows() * columns(); };

		pointer data() noexcept { return data_; }
		const_pointer data() const noexcept { return data_; }

		constexpr reference at(const size_type index)
		{
			if(index >= size())
			{
				throw Matrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		constexpr const_reference at(const size_type index) const
		{
			if (index >= size())
			{
				throw Matrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		constexpr reference at(const size_type row, const size_type column)
		{
			if (row >= rows() || 
				column >= columns())
			{
				throw Matrix_RowColOutOfBounds{ row, column, rows(), columns() };
			}

			return data_[to_index(row, column)];
		}

		constexpr const_reference at(const size_type row, const size_type column) const
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw Matrix_RowColOutOfBounds{ row, column, rows(), columns() };
			}

			return data_[to_index(row, column)];
		}

		constexpr reference operator[](const size_type index)
		{
			return data_[index];
		}

		constexpr const_reference operator[](const size_type index) const
		{
			return data_[index];
		}

		constexpr reference operator()(const size_type row, const size_type column)
		{
			return data_[to_index(row, column)];
		}

		constexpr const_reference operator()(const size_type row, const size_type column) const
		{
			return data_[to_index(row, column)];
		}

		iterator begin() noexcept
		{
			return std::begin(data_);
		}

		constexpr const_iterator begin() const noexcept
		{
			return std::cbegin(data_);
		}

		iterator end() noexcept
		{
			return std::end(data_);
		}

		constexpr const_iterator end() const noexcept
		{
			return std::cend(data_);
		}

		iterator cbegin() noexcept
		{
			return std::cbegin(data_);
		}

		constexpr const_iterator cend() const noexcept
		{
			return std::cend(data_);
		}

		SMatrix& operator+=(const SMatrix& rhs)
		{
			auto rhs_it = rhs.begin();
			for(auto& el : *this)
			{
				el += *rhs_it++;
			}

			return *this;
		}

		SMatrix& operator-=(const SMatrix& rhs)
		{
			auto rhs_it = rhs.begin();
			for (auto& el : *this)
			{
				el -= *rhs_it++;
			}

			return *this;
		}

		SMatrix& operator*=(const value_type& rhs)
		{
			for (auto& el : *this)
			{
				el *= rhs;
			}

			return *this;
		}

		/* OPERATIONS */
		constexpr SMatrix<value_type, columns_, rows_> transpose() const
		{
			SMatrix<value_type, columns_, rows_> result_matrix;

			auto current_matrix_iterator = cbegin();
			for(size_type result_column = 0; result_column < result_matrix.columns(); ++result_column)
			{
				for(size_type result_row = 0; result_row < result_matrix.rows(); ++result_row, ++current_matrix_iterator)
				{
					result_matrix(result_row, result_column) = *current_matrix_iterator;
				}
			}

			return result_matrix;
		}

		value_type data_[rows_ * columns_];

	private:
		constexpr size_type to_index(const size_type row, const size_type column) const noexcept
		{
			return row * columns() + column;
		}
	};

	template <class T, size_t rows_, size_t columns_>
	constexpr SMatrix<T, rows_, columns_> operator+(
		const SMatrix<T, rows_, columns_>& lhs,
		const SMatrix<T, rows_, columns_>& rhs)
	{
		SMatrix<T, rows_, columns_> result_matrix{};
		for(size_t i = 0; i < result_matrix.size(); ++i)
		{
			result_matrix[i] = lhs[i] + rhs[i];
		}

		return result_matrix;
	}

	template <class T, size_t rows_, size_t columns_>
	constexpr SMatrix<T, rows_, columns_> operator-(
		const SMatrix<T, rows_, columns_>& lhs,
		const SMatrix<T, rows_, columns_>& rhs)
	{
		SMatrix<T, rows_, columns_> result_matrix{};
		for (size_t i = 0; i < result_matrix.size(); ++i)
		{
			result_matrix[i] = lhs[i] - rhs[i];
		}

		return result_matrix;
	}

	template <class T, size_t lhs_rows_, size_t common_, size_t rhs_columns_>
	constexpr SMatrix<T, lhs_rows_, rhs_columns_> operator*(
		const SMatrix<T, lhs_rows_, common_>& lhs,
		const SMatrix<T, common_, rhs_columns_>& rhs)
	{
		SMatrix<T, lhs_rows_, rhs_columns_> result_matrix{};

		constexpr auto sum_len = common_;
		for (size_t result_row = 0; result_row < result_matrix.rows(); ++result_row)
		{
			for (size_t result_column = 0; result_column < result_matrix.columns(); ++result_column)
			{
				for (size_t sum_index = 0; sum_index < sum_len; ++sum_index)
				{
					result_matrix(result_row, result_column) += lhs(result_row, sum_index) * rhs(sum_index, result_column);
				}
			}
		}
		return result_matrix;
	}

	template <class T, size_t rows_, size_t columns_>
	constexpr SMatrix<T, rows_, columns_> operator*(
		const SMatrix<T, rows_, columns_>& lhs,
		const T& rhs)
	{
		SMatrix<T, rows_, columns_> result_matrix{};

		for(size_t i = 0; i < lhs.size(); ++i)
		{
			result_matrix[i] = lhs[i] * rhs;
		}

		return result_matrix;
	}


};