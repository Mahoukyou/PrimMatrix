#include "src/DMatrix.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <vector>

// todo, use mocks, lots of repetitive initialization code

template <class T>
struct test_data
{
	using value_type = T;
	using size_type = typename PrimMatrix::DMatrix<T>::size_type;

	test_data(const size_type rows_, const size_type columns_) :
		rows{ rows_ },
		columns{ columns_ },
		size{ rows_ * columns_ }
	{

	}

	const size_type rows, columns, size;
};

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(DMatrix_ConstructionTest, T_001_RowCol)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 3 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		const std::vector<test_type> test_elements(test_data.size, test_type{});
		EXPECT_THAT(matrix, ::testing::ElementsAreArray(test_elements));
	}

	{
		using test_type = int;
		const test_data<test_type> test_data{ 4, 5 };
		constexpr test_type initial_value = 5;

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, initial_value);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		const std::vector<test_type> test_elements(test_data.size, initial_value);
		EXPECT_THAT(matrix, ::testing::ElementsAreArray(test_elements));
	}
}

TEST(DMatrix_ConstructionTest, T_002_RowColVectorInitialized)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, vec);

		EXPECT_EQ(vec.size(), test_data.size);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::vector<test_type> vec{ 1,2,3 };

		try
		{
			const DMatrix<test_type> matrix(test_data.rows, test_data.columns, vec);
			
			// should throw
			EXPECT_TRUE(false);
		}
		catch(const DMatrix_InvalidInitializerSize& e)
		{
			EXPECT_EQ(e.initializer_size(), vec.size());
			EXPECT_EQ(e.matrix_size(), test_data.size);
		}
	}
}

TEST(DMatrix_ConstructionTest, T_003_VectorVerticalOrientation)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };
		const test_data<test_type> test_data{ vec.size(), 1 };

		const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::vertical);

		EXPECT_EQ(vec.size(), test_data.size);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}

	//{
	//	using test_type = int;
	//	const std::vector<test_type> vec{ };

	//	try
	//	{
	//		const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::vertical);

	//		EXPECT_TRUE(false);
	//	}
	//	catch (const DMatrix_ArrayIsEmpty&)
	//	{
	//	}
	//}
}

TEST(DMatrix_ConstructionTest, T_004_VectorHorizontalOrientation)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };
		const test_data<test_type> test_data{ 1, vec.size() };

		const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::horizontal);

		EXPECT_EQ(vec.size(), test_data.size);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}
}

TEST(DMatrix_ConstructionTest, T_005_RowColInitializerList)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, il);

		EXPECT_EQ(il.size(), test_data.size);

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(il));
	}

	{
		using test_type = int;
		const test_data<test_type> test_data{ 3, 2 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, { 0,1,2,3,4,5 });

		EXPECT_EQ(matrix.rows(), test_data.rows);
		EXPECT_EQ(matrix.columns(), test_data.columns);
		EXPECT_EQ(matrix.size(), test_data.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 0,1,2,3,4,5 }));
	}

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };

		try
		{
			const DMatrix<test_type> matrix(test_data.rows, test_data.columns, {4,5,6,2});

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_InvalidInitializerSize& e)
		{
			const DMatrix<test_type>::size_type il_size = 4;
			EXPECT_EQ(e.initializer_size(), il_size);
			EXPECT_EQ(e.matrix_size(), test_data.size);
		}
	}

}

TEST(DMatrix_ConstructionTest, T_006_CopyConstructor)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix{ test_data.rows, test_data.columns, il };

		const auto matrix_copy = matrix;

		EXPECT_EQ(matrix_copy.rows(), test_data.rows);
		EXPECT_EQ(matrix_copy.columns(), test_data.columns);
		EXPECT_EQ(matrix_copy.size(), test_data.size);

		EXPECT_THAT(matrix_copy, ::testing::ElementsAreArray(matrix));
	}
}

TEST(DMatrix_ConstructionTest, T_007_MoveConstructor)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		DMatrix<test_type> matrix{ test_data.rows, test_data.columns, il };

		const auto matrix_moved = std::move(matrix);

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);
		EXPECT_EQ(matrix.size(), 0);

		EXPECT_EQ(matrix_moved.rows(), test_data.rows);
		EXPECT_EQ(matrix_moved.columns(), test_data.columns);
		EXPECT_EQ(matrix_moved.size(), test_data.size);

		EXPECT_THAT(matrix_moved, ::testing::ElementsAreArray(il));
	}
}


TEST(DMatrix_ConstructionTest, T_008_CopyAssignmentOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix{ test_data.rows, test_data.columns, il };
		DMatrix<test_type> matrix_copy{ 1,1 };

		matrix_copy = matrix;

		EXPECT_EQ(matrix_copy.rows(), matrix.rows());
		EXPECT_EQ(matrix_copy.columns(), matrix.columns());
		EXPECT_EQ(matrix_copy.size(), matrix.size());

		EXPECT_THAT(matrix_copy, ::testing::ElementsAreArray(matrix));
	}
}

TEST(DMatrix_ConstructionTest, T_009_MoveAssigmentOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		DMatrix<test_type> matrix{ test_data.rows, test_data.columns, il };
		DMatrix<test_type> matrix_moved{ 1,1, {5} };
		
		matrix_moved = std::move(matrix);

		EXPECT_EQ(matrix.rows(), 1);
		EXPECT_EQ(matrix.columns(), 1);
		EXPECT_EQ(matrix.size(), 1);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 5 }));

		EXPECT_EQ(matrix_moved.rows(), test_data.rows);
		EXPECT_EQ(matrix_moved.columns(), test_data.columns);
		EXPECT_EQ(matrix_moved.size(), test_data.size);

		EXPECT_THAT(matrix_moved, ::testing::ElementsAreArray(il));

		// move to itself
		matrix_moved = std::move(matrix_moved);
		EXPECT_EQ(matrix_moved.rows(), test_data.rows);
		EXPECT_EQ(matrix_moved.columns(), test_data.columns);
		EXPECT_EQ(matrix_moved.size(), test_data.size);

		EXPECT_THAT(matrix_moved, ::testing::ElementsAreArray(il));
	}
}

TEST(DMatrix_AtTest, T_001_IndexNonConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		DMatrix<test_type> matrix{ test_data.rows, test_data.columns, il };


		for (DMatrix<test_type>::size_type i = 0; i < matrix.size(); ++i)
		{
			EXPECT_EQ(matrix.at(i), *(il.begin() + i));
		}

		const auto index_out_of_bounds = test_data.size + 1;
		try
		{
			matrix.at(index_out_of_bounds);

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), index_out_of_bounds);
			EXPECT_EQ(e.matrix_size(), test_data.size);
		}
	}
}

TEST(DMatrix_AtTest, T_002_IndexConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, il);

		for (DMatrix<test_type>::size_type i = 0; i < matrix.size(); ++i)
		{
			EXPECT_EQ(matrix.at(i), *(il.begin() + i));
		}


		const auto index_out_of_bounds = test_data.size + 1;
		try
		{
			matrix.at(index_out_of_bounds);

			EXPECT_TRUE(false);
		}
		catch(const DMatrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), index_out_of_bounds);
			EXPECT_EQ(e.matrix_size(), matrix.size());
		}
	}
}


TEST(DMatrix_AtTest, T_003_RowColNonConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		DMatrix<test_type> matrix(test_data.rows, test_data.columns, il);

		for (DMatrix<test_type>::size_type row = 0, index = 0; row < matrix.rows(); ++row)
		{
			for (DMatrix<test_type>::size_type column = 0; column < matrix.columns(); ++column, ++index)
			{
				EXPECT_EQ(matrix.at(row, column), *(il.begin() + index));
			}
		}

		{
			const auto row_out_of_bounds = test_data.rows;
			const auto column = test_data.columns - 1;
			try
			{
				matrix.at(row_out_of_bounds, column);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row_out_of_bounds);
				EXPECT_EQ(e.columns(), column);
				EXPECT_EQ(e.matrix_rows(), matrix.rows());
				EXPECT_EQ(e.matrix_columns(), matrix.columns());
			}
		}

		{
			const auto row = test_data.rows - 1;
			const auto column_out_of_bounds = test_data.columns;
			try
			{
				matrix.at(row, column_out_of_bounds);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row);
				EXPECT_EQ(e.columns(), column_out_of_bounds);
				EXPECT_EQ(e.matrix_rows(), matrix.rows());
				EXPECT_EQ(e.matrix_columns(), matrix.columns());
			}
		}
	}
}

TEST(DMatrix_AtTest, T_004_RowColConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, il);

		for (DMatrix<test_type>::size_type row = 0, index = 0; row < matrix.rows(); ++row)
		{
			for (DMatrix<test_type>::size_type column = 0; column < matrix.columns(); ++column, ++index)
			{
				EXPECT_EQ(matrix.at(row, column), *(il.begin() + index));
			}
		}

		{
			const auto row_out_of_bounds = test_data.rows;
			const auto column = test_data.columns - 1;
			try
			{
				matrix.at(row_out_of_bounds, column);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row_out_of_bounds);
				EXPECT_EQ(e.columns(), column);
				EXPECT_EQ(e.matrix_rows(), matrix.rows());
				EXPECT_EQ(e.matrix_columns(), matrix.columns());
			}
		}

		{
			const auto row = test_data.rows - 1;
			const auto column_out_of_bounds = test_data.columns;
			try
			{
				matrix.at(row, column_out_of_bounds);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row);
				EXPECT_EQ(e.columns(), column_out_of_bounds);
				EXPECT_EQ(e.matrix_rows(), matrix.rows());
				EXPECT_EQ(e.matrix_columns(), matrix.columns());
			}
		}
	}
}

TEST(DMatrix_OperatorTests, T_001_AdditionOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const DMatrix<test_type> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> m2{ 2,3,{6,5,1,3,2,1} };

		const DMatrix<test_type> addition_result = m1 + m2;

		EXPECT_EQ(addition_result.rows(), 2);
		EXPECT_EQ(addition_result.columns(), 3);
		EXPECT_EQ(addition_result.size(), 6);

		EXPECT_THAT(addition_result, ::testing::ElementsAre(7, 7, 2, 7, 7, 7));
	}

	{
		using test_type = int;

		const DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			auto tmp = matrix + matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch(const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::addition);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			auto tmp = matrix + matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::addition);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}

TEST(DMatrix_OperatorTests, T_002_AdditionEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		DMatrix<test_type> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> m2{ 2,3,{6,5,1,3,2,1} };

		m1 += m2;

		EXPECT_EQ(m1.rows(), 2);
		EXPECT_EQ(m1.columns(), 3);
		EXPECT_EQ(m1.size(), 6);

		EXPECT_THAT(m1, ::testing::ElementsAre(7, 7, 2, 7, 7, 7));
	}

	{
		using test_type = int;

		DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			matrix += matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::addition);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			matrix += matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::addition);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}

TEST(DMatrix_OperatorTests, T_003_SubtractOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const DMatrix<test_type> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> m2{ 2,3,{6,5,4,3,2,1} };

		const DMatrix<test_type> subtraction_result = m1 - m2;

		EXPECT_EQ(subtraction_result.rows(), 2);
		EXPECT_EQ(subtraction_result.columns(), 3);
		EXPECT_EQ(subtraction_result.size(), 6);

		EXPECT_THAT(subtraction_result, ::testing::ElementsAre(-5, -3, -3, 1, 3, 5));
	}

	{
		using test_type = int;

		const DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			auto tmp = matrix - matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::subtraction);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			auto tmp = matrix - matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::subtraction);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}

TEST(DMatrix_OperatorTests, T_003_SubtractEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		DMatrix<test_type> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> m2{ 2,3,{6,5,4,3,2,1} };

		m1 -= m2;

		EXPECT_EQ(m1.rows(), 2);
		EXPECT_EQ(m1.columns(), 3);
		EXPECT_EQ(m1.size(), 6);

		EXPECT_THAT(m1, ::testing::ElementsAre(-5, -3, -3, 1, 3, 5));
	}

	{
		using test_type = int;

		DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			matrix -= matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::subtraction);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			matrix -= matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::subtraction);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}


TEST(DMatrix_OperatorTests, T_004_MultiplicationOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const test_data<test_type> test_data(2, 3);
		const DMatrix<test_type> m1{ test_data.rows, test_data.columns, {1,2,3,4,5,6} };
		const DMatrix<test_type> m2{ test_data.columns, test_data.rows, {1,2,3,4,5,6} };

		const DMatrix<test_type> multiplication_result = m1 * m2;

		EXPECT_EQ(multiplication_result.rows(), test_data.rows);
		EXPECT_EQ(multiplication_result.columns(), test_data.rows);
		EXPECT_EQ(multiplication_result.size(), test_data.rows * test_data.rows);

		EXPECT_THAT(multiplication_result, ::testing::ElementsAre(22, 28, 49, 64));
	}

	{
		using test_type = int;

		const DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			auto tmp = matrix * matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::multiplication);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			auto tmp = matrix * matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::multiplication);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}

TEST(DMatrix_OperatorTests, T_005_MultiplicationEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const test_data<test_type> test_data(2, 3);
		DMatrix<test_type> m1{ test_data.rows, test_data.columns, {1,2,3,4,5,6} };
		const DMatrix<test_type> m2{ test_data.columns, test_data.rows, {1,2,3,4,5,6} };

		m1 *= m2;

		EXPECT_EQ(m1.rows(), test_data.rows);
		EXPECT_EQ(m1.columns(), test_data.rows);
		EXPECT_EQ(m1.size(), test_data.rows * test_data.rows);

		EXPECT_THAT(m1, ::testing::ElementsAre(22, 28, 49, 64));
	}

	{
		using test_type = int;

		DMatrix<test_type> matrix{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<test_type> matrix_diff_rows{ 1,3,{1,2,3} };
		const DMatrix<test_type> matrix_diff_columns{ 2,1,{1,2} };

		try
		{
			matrix *= matrix_diff_rows;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::multiplication);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_rows.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_rows.columns());
		}

		try
		{
			matrix *= matrix_diff_columns;

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_OperationMatrixMismatch& e)
		{
			EXPECT_EQ(e.operation(), DMatrix_OperationMatrixMismatch::EOperation::multiplication);
			EXPECT_EQ(e.lhs_rows(), matrix.rows());
			EXPECT_EQ(e.lhs_columns(), matrix.columns());
			EXPECT_EQ(e.rhs_rows(), matrix_diff_columns.rows());
			EXPECT_EQ(e.rhs_columns(), matrix_diff_columns.columns());
		}
	}
}

TEST(DMatrix_OperatorTests, T_006_ScalarMultiplicationOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const test_data<test_type> test_data(2, 3);
		const DMatrix<test_type> matrix{ test_data.rows, test_data.columns, {1,2,3,4,5,6} };

		const DMatrix<test_type> scalar_mul_result = matrix * 3;
		const DMatrix<test_type> scalar_mul_result2 = 3 * matrix;
		const DMatrix<test_type> scalar_mul_result3 = 3 * matrix * 3;


		EXPECT_THAT(scalar_mul_result, ::testing::ElementsAre(3, 6, 9, 12, 15, 18));
		EXPECT_THAT(scalar_mul_result2, ::testing::ElementsAre(3, 6, 9, 12, 15, 18));
		EXPECT_THAT(scalar_mul_result3, ::testing::ElementsAre(9, 18, 27, 36, 45, 54));
	}
}

TEST(DMatrix_OperatorTests, T_007_ScalarMultiplicationEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const test_data<test_type> test_data(2, 3);
		DMatrix<test_type> matrix{ test_data.rows, test_data.columns, {1,2,3,4,5,6} };

		matrix *= 3;

		EXPECT_THAT(matrix, ::testing::ElementsAre(3, 6, 9, 12, 15, 18));
	}
}

TEST(DMatrix_OperatorTests, T_008_EqualsAndNotEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		const DMatrix<test_type> matrix{ 2, 3, {1,2,3,4,5,6} };
		const DMatrix<test_type> matrix_diff_data{ 2, 3, {1,5,4,3,2,1} };
		const DMatrix<test_type> matrix_diff_rows{ 1, 3, {1,2,3} };
		const DMatrix<test_type> matrix_diff_cols{ 2,2,{1,2,3,4} };
		const auto copy{ matrix };
			
		EXPECT_TRUE(matrix == copy);
		EXPECT_FALSE(matrix == matrix_diff_data);
		EXPECT_FALSE(matrix == matrix_diff_rows);
		EXPECT_FALSE(matrix == matrix_diff_cols);
		EXPECT_FALSE(matrix_diff_rows == matrix_diff_cols);

		EXPECT_FALSE(matrix != copy);
		EXPECT_TRUE(matrix != matrix_diff_data);
		EXPECT_TRUE(matrix != matrix_diff_rows);
		EXPECT_TRUE(matrix != matrix_diff_cols);
		EXPECT_TRUE(matrix_diff_rows != matrix_diff_cols);
	}
}


TEST(DMatrix_Operations, T_001_Transpose)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const test_data<test_type> test_data{ 5, 2 };

		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, vec);
		const DMatrix<test_type> transposed = matrix.transpose();

		EXPECT_EQ(transposed.rows(), test_data.columns);
		EXPECT_EQ(transposed.columns(), test_data.rows);
		EXPECT_EQ(transposed.size(), test_data.size);

		EXPECT_THAT(transposed, ::testing::ElementsAre(1, 3, 5, 7, 9, 2, 4, 6, 8, 10));
	}

	{
		using test_type = int;
		const test_data<test_type> test_data{ 2, 5 };

		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(test_data.rows, test_data.columns, vec);
		const DMatrix<test_type> transposed = matrix.transpose();

		EXPECT_EQ(transposed.rows(), test_data.columns);
		EXPECT_EQ(transposed.columns(), test_data.rows);
		EXPECT_EQ(transposed.size(), test_data.size);

		EXPECT_THAT(transposed, ::testing::ElementsAre(1, 6, 2, 7, 3, 8, 4, 9, 5, 10));
	}
}

TEST(DMatrix_Operations, T_002_CreateIdentityMatrix)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const DMatrix<test_type>::value_type size = 3;
		const auto identityMatrix = DMatrix<test_type>::create_identity_matrix(size);

		EXPECT_EQ(identityMatrix.rows(), size);
		EXPECT_EQ(identityMatrix.columns(), size);
		EXPECT_EQ(identityMatrix.size(), size * size);

		EXPECT_THAT(identityMatrix, ::testing::ElementsAre(1, 0, 0, 0, 1, 0, 0, 0, 1));
	}


	{
		using test_type = int;
		const DMatrix<test_type>::value_type size = 1;
		const auto identityMatrix = DMatrix<test_type>::create_identity_matrix(size);

		EXPECT_EQ(identityMatrix.rows(), size);
		EXPECT_EQ(identityMatrix.columns(), size);
		EXPECT_EQ(identityMatrix.size(), size * size);

		EXPECT_THAT(identityMatrix, ::testing::ElementsAre(1));
	}
}