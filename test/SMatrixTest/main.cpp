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

		// todo exception for at method
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

		// todo exception for at method
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