#include "src/SMatrix.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <array>

using namespace PrimMatrix;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(ConstructionTest, AggragateConstrcution)
{
	{
		constexpr SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);
		EXPECT_EQ(matrix.size(), 6);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6 }));
	}

	{
		constexpr SMatrix<int, 2, 3> matrix{ };

		EXPECT_EQ(matrix.rows(), 2);
		EXPECT_EQ(matrix.columns(), 3);
		EXPECT_EQ(matrix.size(), 6);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 0, 0, 0, 0, 0, 0 }));
	}
}

TEST(ConstructionTest, CopyConstruction)
{
	{
		constexpr SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		constexpr auto copy_matrix{ matrix };

		EXPECT_EQ(copy_matrix.rows(), 2);
		EXPECT_EQ(copy_matrix.columns(), 3);
		EXPECT_EQ(copy_matrix.size(), 6);

		EXPECT_THAT(copy_matrix, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6 }));
	}
}

TEST(ConstructionTest, MoveConstruction)
{
	{
		SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };

		// move has no effect, but still worth to check if [copied]
		const SMatrix<int, 2, 3> copy_matrix{ std::move(matrix) };

		EXPECT_EQ(copy_matrix.rows(), 2);
		EXPECT_EQ(copy_matrix.columns(), 3);
		EXPECT_EQ(copy_matrix.size(), 6);

		EXPECT_THAT(copy_matrix, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6 }));
	}
}

TEST(ConstructionTest, CopyAssigment)
{
	{
		constexpr SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		SMatrix<int, 2, 3> copy_matrix{ };

		copy_matrix = matrix;

		EXPECT_EQ(copy_matrix.rows(), 2);
		EXPECT_EQ(copy_matrix.columns(), 3);
		EXPECT_EQ(copy_matrix.size(), 6);

		EXPECT_THAT(copy_matrix, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6 }));
	}
}

TEST(ConstructionTest, MoveAssigment)
{
	{
		constexpr SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		SMatrix<int, 2, 3> copy_matrix{ };

		// move has no effect, but still worth to check if copied
		copy_matrix = std::move(matrix);

		EXPECT_EQ(copy_matrix.rows(), 2);
		EXPECT_EQ(copy_matrix.columns(), 3);
		EXPECT_EQ(copy_matrix.size(), 6);

		EXPECT_THAT(copy_matrix, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6 }));
	}
}

TEST(DataAccessTest, ByIndex)
{
	{
		constexpr std::array<int, 6> test_data{ 1, 2, 3, 4, 5, 6 };
		
		SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		const SMatrix<int, 2, 3> const_matrix{ matrix };

		for(size_t index = 0; index < matrix.size(); ++index)
		{
			EXPECT_EQ(matrix.at(index), test_data[index]);
			EXPECT_EQ(const_matrix.at(index), test_data[index]);

			EXPECT_EQ(matrix[index], test_data[index]);
			EXPECT_EQ(const_matrix[index], test_data[index]);
		}

		const size_t index = matrix.size();
		try
		{
			matrix.at(index);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), index);
			EXPECT_EQ(e.matrix_size(), matrix.size());
		}

		try
		{
			const_matrix.at(index);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), index);
			EXPECT_EQ(e.matrix_size(), matrix.size());
		}
	}
}

TEST(DataAccessTest, ByRowAndColumn)
{
	{
		constexpr std::array<int, 6> test_data{ 1, 2, 3, 4, 5, 6 };

		SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		const SMatrix<int, 2, 3> const_matrix{ matrix };

		for (size_t row = 0, index = 0; row < matrix.rows(); ++row)
		{
			for (size_t column = 0; column < matrix.columns(); ++column, ++index)
			{
				EXPECT_EQ(matrix.at(row, column), test_data[index]);
				EXPECT_EQ(const_matrix.at(row, column), test_data[index]);

				EXPECT_EQ(matrix(row, column), test_data[index]);
				EXPECT_EQ(const_matrix(row, column), test_data[index]);
			}
		}

		const size_t row_out_of_bounds = matrix.rows();
		const size_t column_out_of_bounds = matrix.columns();
		const size_t row = 0;
		const size_t column = 0;
		try
		{
			matrix.at(row_out_of_bounds, column);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_RowColOutOfBounds& e)
		{
			EXPECT_EQ(e.rows(), row_out_of_bounds);
			EXPECT_EQ(e.columns(), column);
			EXPECT_EQ(e.matrix_rows(), matrix.rows());
			EXPECT_EQ(e.matrix_columns(), matrix.columns());
		}

		try
		{
			matrix.at(row, column_out_of_bounds);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_RowColOutOfBounds& e)
		{
			EXPECT_EQ(e.rows(), row);
			EXPECT_EQ(e.columns(), column_out_of_bounds);
			EXPECT_EQ(e.matrix_rows(), matrix.rows());
			EXPECT_EQ(e.matrix_columns(), matrix.columns());
		}

		try
		{
			const_matrix.at(row_out_of_bounds, column);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_RowColOutOfBounds& e)
		{
			EXPECT_EQ(e.rows(), row_out_of_bounds);
			EXPECT_EQ(e.columns(), column);
			EXPECT_EQ(e.matrix_rows(), const_matrix.rows());
			EXPECT_EQ(e.matrix_columns(), const_matrix.columns());
		}

		try
		{
			const_matrix.at(row, column_out_of_bounds);

			EXPECT_FALSE(true);
		}
		catch (const Matrix_RowColOutOfBounds& e)
		{
			EXPECT_EQ(e.rows(), row);
			EXPECT_EQ(e.columns(), column_out_of_bounds);
			EXPECT_EQ(e.matrix_rows(), const_matrix.rows());
			EXPECT_EQ(e.matrix_columns(), const_matrix.columns());
		}
	}
}

TEST(IteratorTest, NormalIteartors)
{
	{
		constexpr std::array<int, 6> test_data{ 1, 2, 3, 4, 5, 6 };

		constexpr SMatrix<int, 2, 3> matrix{ 1, 2, 3, 4, 5, 6 };
		constexpr SMatrix<int, 2, 3> const_matrix{ matrix };

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(test_data));
		EXPECT_THAT(const_matrix, ::testing::ElementsAreArray(test_data));
	}
}

TEST(OperatorTest, AddEqualOperator)
{
	{
		SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		constexpr SMatrix<int, 2, 3> matrix2{ 6, 5, 4, 3, 2, 1 };

		matrix1 += matrix2;

		EXPECT_THAT(matrix1, ::testing::ElementsAreArray({ 7, 7, 7, 7, 7, 7 }));
	}
}

TEST(OperatorTest, SubtractEqualOperator)
{
	{
		SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		const SMatrix<int, 2, 3> matrix2{ 6, 5, 4, 3, 2, 1 };

		matrix1 -= matrix2;

		EXPECT_THAT(matrix1, ::testing::ElementsAreArray({ -5, -3, -1, 1, 3, 5 }));
	}
}

TEST(OperatorTest, MultiplayEqualScalarOperator)
{
	{
		SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		matrix1 *= 4;

		EXPECT_THAT(matrix1, ::testing::ElementsAreArray({ 4, 8, 12, 16, 20, 24 }));
	}
}

TEST(OperatorTest, AdditionOperator)
{
	{
		constexpr SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		constexpr SMatrix<int, 2, 3> matrix2{ 6, 5, 4, 3, 2, 1 };

		constexpr SMatrix<int, 2, 3> addition_result = matrix1 + matrix2;

		EXPECT_THAT(addition_result, ::testing::ElementsAreArray({ 7, 7, 7, 7, 7, 7 }));
	}
}

TEST(OperatorTest, SubtractionOperator)
{
	{
		constexpr SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		constexpr SMatrix<int, 2, 3> matrix2{ 6, 5, 4, 3, 2, 1 };

		constexpr SMatrix<int, 2, 3> subtraction_result = matrix1 - matrix2;

		EXPECT_THAT(subtraction_result, ::testing::ElementsAreArray({ -5, -3, -1, 1, 3, 5 }));
	}
}		

TEST(OperatorTest, MultiplicationOperator)
{
	{
		constexpr SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		constexpr SMatrix<int, 3, 2> matrix2{ 6, 5, 4, 3, 2, 1 };

		constexpr SMatrix<int, 2, 2> multiplication_result = matrix1 * matrix2;

		EXPECT_THAT(multiplication_result, ::testing::ElementsAreArray({ 20, 14, 56, 41}));
	}
}

TEST(OperatorTest, MultiplicationByScalarOperator)
{
	{
		constexpr SMatrix<int, 2, 3> matrix1{ 1, 2, 3, 4, 5, 6 };
		
		constexpr SMatrix<int, 2, 3> multiplication_result = matrix1 * 4;

		EXPECT_THAT(multiplication_result, ::testing::ElementsAreArray({ 4, 8, 12, 16, 20, 24 }));
	}
}

TEST(OperationsTests, Transpose)
{
	{
		constexpr SMatrix<int, 2, 5> matrix{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		
		constexpr SMatrix<int, 5, 2> transposed = matrix.transpose();

		EXPECT_THAT(transposed, ::testing::ElementsAre(1, 6, 2, 7, 3, 8, 4, 9, 5, 10));
	}
}

TEST(OperationsTests, Splice)
{
	{
		constexpr SMatrix<int, 3, 4> matrix{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
		constexpr SMatrix<int, 2, 2> spliced = matrix.splice<1, 2, 2, 2>();

		EXPECT_THAT(spliced, ::testing::ElementsAre(7, 8, 11, 12));
	}
}