#pragma once

#include <vector>

#include "DMatrix_exception.h"

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
		
		//tmp for now
		using iterator = typename std::vector<T>::iterator;
		using const_iterator = typename std::vector<T>::const_iterator;

		enum class EOrientation 
		{ 
			horizontal, 
			vertical 
		};

		/* CONSTRUCTION || DESTRUCTION */
		explicit DMatrix(const size_type row_count, const size_type column_count) :
			row_count_{ row_count },
			column_count_{ column_count },
			data_(row_count_ * column_count_)
		{ 
			
		}

		explicit DMatrix(const size_type row_count, const size_type column_count, const value_type& initial_value) :
			row_count_{ row_count },
			column_count_{ column_count }, 
			data_(row_count_ * column_count_, initial_value)
		{

		}

		explicit DMatrix(const size_type row_count, const size_type column_count, const std::vector<value_type>& arr) :
			row_count_{ row_count },
			column_count_{ column_count }
		{
			const size_type size = rows() * columns();
			if(arr.size() != size)
			{
				throw DMatrix_InvalidInitializerSize{ arr.size(), size };
			}

			data_ = arr;
		}

		explicit DMatrix(const size_type row_count, const size_type column_count, std::initializer_list<value_type> il) : 
			row_count_{ row_count },
			column_count_{ column_count }
		{
			const size_type size = rows() * columns();
			if (il.size() != size)
			{
				throw DMatrix_InvalidInitializerSize{ il.size(), size };
			}

			data_ = std::move(il);
		}

		explicit DMatrix(const std::vector<value_type>& arr, const EOrientation orientation) :
			row_count_{ orientation == EOrientation::vertical ? arr.size() : 1 },
			column_count_{ orientation == EOrientation::horizontal ? arr.size() : 1 },
			data_{ arr }
		{
		}

		virtual ~DMatrix() = default;

		DMatrix(const DMatrix&) = default;
		DMatrix(DMatrix&& rhs) noexcept :
			row_count_{ rhs.row_count_ },
			column_count_{ rhs.column_count_ },
			data_{ std::move(rhs.data_) }
		{
			rhs.row_count_ = rhs.column_count_ = 0;
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
			swap(lhs.column_count_, rhs.column_count_);
			swap(lhs.data_, rhs.data_);
		}

		/* ACCESSORS */
		size_type rows() const noexcept { return row_count_; }
		size_type columns() const noexcept { return column_count_; }
		size_type size() const noexcept { return data_.size(); }

		pointer data() noexcept { return data_.data(); }
		const_pointer data() const noexcept { return data_.data(); }

		reference at(const size_type index)
		{
			if (index >= size())
			{
				throw DMatrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		const_reference at(const size_type index) const
		{
			if (index >= size())
			{
				throw DMatrix_IndexOutOfBounds{ index, size() };
			}

			return data_[index];
		}

		reference at(const size_type row, const size_type column)
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw DMatrix_RowColOutOfBounds{ row, column, rows(), columns() };
			}

			return data_[to_index(row, column)];
		}

		const_reference at(const size_type row, const size_type column) const
		{
			if (row >= rows() ||
				column >= columns())
			{
				throw DMatrix_RowColOutOfBounds{row, column, rows(), columns()};
			}

			return data_[to_index(row, column)];
		}

		/* tmp ITERATORS -- vector ones for now */
		iterator begin() { return data_.begin(); }
		const_iterator begin() const { return data_.begin(); }
		iterator end() { return data_.end(); }
		const_iterator end() const { return data_.end(); }

		const_iterator cbegin() const { return data_.cbegin(); }
		const_iterator cend() const { return data_.cend(); }

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
				throw DMatrix_OperationMatrixMismatch {
					DMatrix_OperationMatrixMismatch::EOperation::addition,
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
				throw DMatrix_OperationMatrixMismatch {
					DMatrix_OperationMatrixMismatch::EOperation::subtraction,
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
			DMatrix result_matrix{ column_count_, row_count_ };

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
		size_type to_index(const size_type row, const size_type column) const
		{
			return row * column_count_ + column;
		}

		size_type row_count_, column_count_;
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
			throw DMatrix_OperationMatrixMismatch {
				DMatrix_OperationMatrixMismatch::EOperation::addition,
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
			throw DMatrix_OperationMatrixMismatch {
				DMatrix_OperationMatrixMismatch::EOperation::subtraction,
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
			throw DMatrix_OperationMatrixMismatch { 
				DMatrix_OperationMatrixMismatch::EOperation::multiplication, 
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