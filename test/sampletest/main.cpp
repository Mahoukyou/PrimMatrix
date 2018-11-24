#include "src/DMatrix.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <vector>

#define NDEBUG

template <class T>
struct TestData
{
	using value_type = T;
	using size_type = typename PrimMatrix::DMatrix<T>::size_type;


	TestData(const size_type _rows, const size_type _columns) :
		rows{ _rows },
		columns{ _columns },
		size{ _rows * _columns }
	{

	}

	const size_type rows, columns, size;
};

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(DMatrix_ConstructionTest, T_001_RowCol)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 3 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		const std::vector<test_type> testElements(testData.size, test_type{});
		EXPECT_THAT(matrix, ::testing::ElementsAreArray(testElements));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 4, 5 };
		constexpr test_type initialValue = 5;

		const DMatrix<test_type> matrix(testData.rows, testData.columns, initialValue);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		const std::vector<test_type> testElements(testData.size, initialValue);
		EXPECT_THAT(matrix, ::testing::ElementsAreArray(testElements));
	}
}

TEST(DMatrix_ConstructionTest, T_002_RowColVectorInitialized)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns, vec);

		EXPECT_EQ(vec.size(), testData.size);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::vector<test_type> vec{ };

		try
		{
			const DMatrix<test_type> matrix(testData.rows, testData.columns, vec);
			
			// should throw
			EXPECT_TRUE(false);
		}
		catch(const DMatrix_ArrayIsEmpty&)
		{
			// ... nothing to test in here, except that we got it
		}
	}
}

TEST(DMatrix_ConstructionTest, T_003_VectorVerticalOrientation)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };
		const TestData<test_type> testData{ vec.size(), 1 };

		const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::Vertical);

		EXPECT_EQ(vec.size(), testData.size);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::vector<test_type> vec{ };

		try
		{
			const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::Vertical);

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_ArrayIsEmpty&)
		{
		}
	}
}

TEST(DMatrix_ConstructionTest, T_004_VectorHorizontalOrientation)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };
		const TestData<test_type> testData{ 1, vec.size() };

		const DMatrix<test_type> matrix(vec, DMatrix<test_type>::EOrientation::Horizontal);

		EXPECT_EQ(vec.size(), testData.size);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(vec));
	}
}

TEST(DMatrix_ConstructionTest, T_005_RowColInitializerList)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns, il);

		EXPECT_EQ(il.size(), testData.size);

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray(il));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 3, 2 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns, { 0,1,2,3,4,5 });

		EXPECT_EQ(matrix.rows(), testData.rows);
		EXPECT_EQ(matrix.columns(), testData.columns);
		EXPECT_EQ(matrix.size(), testData.size);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 0,1,2,3,4,5 }));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };

		try
		{
			const DMatrix<test_type> matrix(testData.rows, testData.columns, {});

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_ArrayIsEmpty&)
		{
		}
	}

}

TEST(DMatrix_ConstructionTest, T_006_CopyConstructor)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix{ testData.rows, testData.columns, il };

		const auto matrixCopy = matrix;

		EXPECT_EQ(matrixCopy.rows(), testData.rows);
		EXPECT_EQ(matrixCopy.columns(), testData.columns);
		EXPECT_EQ(matrixCopy.size(), testData.size);

		EXPECT_THAT(matrixCopy, ::testing::ElementsAreArray(matrix));
	}
}

TEST(DMatrix_ConstructionTest, T_007_MoveConstructor)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		DMatrix<test_type> matrix{ testData.rows, testData.columns, il };

		const auto matrixMoved = std::move(matrix);

		EXPECT_EQ(matrix.rows(), 0);
		EXPECT_EQ(matrix.columns(), 0);
		EXPECT_EQ(matrix.size(), 0);

		EXPECT_EQ(matrixMoved.rows(), testData.rows);
		EXPECT_EQ(matrixMoved.columns(), testData.columns);
		EXPECT_EQ(matrixMoved.size(), testData.size);

		EXPECT_THAT(matrixMoved, ::testing::ElementsAreArray(il));
	}
}


TEST(DMatrix_ConstructionTest, T_008_CopyAssignmentOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix{ testData.rows, testData.columns, il };
		DMatrix<test_type> matrixCopy{ 1,1 };

		matrixCopy = matrix;

		EXPECT_EQ(matrixCopy.rows(), matrix.rows());
		EXPECT_EQ(matrixCopy.columns(), matrix.columns());
		EXPECT_EQ(matrixCopy.size(), matrix.size());

		EXPECT_THAT(matrixCopy, ::testing::ElementsAreArray(matrix));
	}
}

TEST(DMatrix_ConstructionTest, T_009_MoveAssigmentOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };
		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };

		DMatrix<test_type> matrix{ testData.rows, testData.columns, il };
		DMatrix<test_type> matrixMoved{ 1,1, {5} };
		
		matrixMoved = std::move(matrix);

		EXPECT_EQ(matrix.rows(), 1);
		EXPECT_EQ(matrix.columns(), 1);
		EXPECT_EQ(matrix.size(), 1);

		EXPECT_THAT(matrix, ::testing::ElementsAreArray({ 5 }));

		EXPECT_EQ(matrixMoved.rows(), testData.rows);
		EXPECT_EQ(matrixMoved.columns(), testData.columns);
		EXPECT_EQ(matrixMoved.size(), testData.size);

		EXPECT_THAT(matrixMoved, ::testing::ElementsAreArray(il));

		matrixMoved = std::move(matrixMoved);
		EXPECT_EQ(matrixMoved.rows(), testData.rows);
		EXPECT_EQ(matrixMoved.columns(), testData.columns);
		EXPECT_EQ(matrixMoved.size(), testData.size);

		EXPECT_THAT(matrixMoved, ::testing::ElementsAreArray(il));
	}
}

TEST(DMatrix_AtTest, T_001_IndexNonConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		DMatrix<test_type> matrix{ testData.rows, testData.columns, il };

		for (TestData<test_type>::size_type i = 0; i < matrix.size(); ++i)
		{
			EXPECT_EQ(matrix.at(i), *(il.begin() + i));
		}

		const auto indexOutofBounds = testData.size + 1;
		try
		{
			matrix.at(indexOutofBounds);

			EXPECT_TRUE(false);
		}
		catch (const DMatrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), indexOutofBounds);
			EXPECT_EQ(e.matrixSize(), testData.size);
		}
	}
}

TEST(DMatrix_AtTest, T_002_IndexConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		const DMatrix<test_type> matrix(testData.rows, testData.columns, il);

		for (TestData<test_type>::size_type i = 0; i < matrix.size(); ++i)
		{
			EXPECT_EQ(matrix.at(i), *(il.begin() + i));
		}


		const auto indexOutofBounds = testData.size + 1;
		try
		{
			matrix.at(indexOutofBounds);

			EXPECT_TRUE(false);
		}
		catch(const DMatrix_IndexOutOfBounds& e)
		{
			EXPECT_EQ(e.index(), indexOutofBounds);
			EXPECT_EQ(e.matrixSize(), matrix.size());
		}
	}
}


TEST(DMatrix_AtTest, T_003_RowColNonConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		DMatrix<test_type> matrix(testData.rows, testData.columns, il);

		for (TestData<test_type>::size_type row = 0, index = 0; row < matrix.rows(); ++row)
		{
			for (TestData<test_type>::size_type column = 0; column < matrix.columns(); ++column, ++index)
			{
				EXPECT_EQ(matrix.at(row, column), *(il.begin() + index));
			}
		}

		{
			const auto rowOutOfBounds = testData.rows;
			const auto column = testData.columns - 1;
			try
			{
				matrix.at(rowOutOfBounds, column);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), rowOutOfBounds);
				EXPECT_EQ(e.columns(), column);
				EXPECT_EQ(e.matrixRows(), matrix.rows());
				EXPECT_EQ(e.matrixColumns(), matrix.columns());
			}
		}

		{
			const auto row = testData.rows - 1;
			const auto columnOutOfBounds = testData.columns;
			try
			{
				matrix.at(row, columnOutOfBounds);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row);
				EXPECT_EQ(e.columns(), columnOutOfBounds);
				EXPECT_EQ(e.matrixRows(), matrix.rows());
				EXPECT_EQ(e.matrixColumns(), matrix.columns());
			}
		}
	}
}

TEST(DMatrix_AtTest, T_004_RowColConst)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 5, 2 };

		const std::initializer_list<test_type> il{ 1,2,3,4,5,6,7,8,9,10 };
		const DMatrix<test_type> matrix(testData.rows, testData.columns, il);

		for (TestData<test_type>::size_type row = 0, index = 0; row < matrix.rows(); ++row)
		{
			for (TestData<test_type>::size_type column = 0; column < matrix.columns(); ++column, ++index)
			{
				EXPECT_EQ(matrix.at(row, column), *(il.begin() + index));
			}
		}

		{
			const auto rowOutOfBounds = testData.rows;
			const auto column = testData.columns - 1;
			try
			{
				matrix.at(rowOutOfBounds, column);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), rowOutOfBounds);
				EXPECT_EQ(e.columns(), column);
				EXPECT_EQ(e.matrixRows(), matrix.rows());
				EXPECT_EQ(e.matrixColumns(), matrix.columns());
			}
		}

		{
			const auto row = testData.rows - 1;
			const auto columnOutOfBounds = testData.columns;
			try
			{
				matrix.at(row, columnOutOfBounds);

				EXPECT_TRUE(false);
			}
			catch (const DMatrix_RowColOutOfBounds& e)
			{
				EXPECT_EQ(e.rows(), row);
				EXPECT_EQ(e.columns(), columnOutOfBounds);
				EXPECT_EQ(e.matrixRows(), matrix.rows());
				EXPECT_EQ(e.matrixColumns(), matrix.columns());
			}
		}
	}
}

TEST(DMatrix_OperatorTests, T_001_AdditionOperator)
{
	using namespace PrimMatrix;

	{
		using testType = int;

		const DMatrix<testType> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<testType> m2{ 2,3,{6,5,1,3,2,1} };

		const DMatrix<testType> additionResult = m1 + m2;

		EXPECT_EQ(additionResult.rows(), 2);
		EXPECT_EQ(additionResult.columns(), 3);
		EXPECT_EQ(additionResult.size(), 6);

		EXPECT_THAT(additionResult, ::testing::ElementsAre(7, 7, 2, 7, 7, 7));

		// todo exception
	}
}

TEST(DMatrix_OperatorTests, T_002_AdditionEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using testType = int;

		DMatrix<testType> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<testType> m2{ 2,3,{6,5,1,3,2,1} };

		m1 += m2;

		EXPECT_EQ(m1.rows(), 2);
		EXPECT_EQ(m1.columns(), 3);
		EXPECT_EQ(m1.size(), 6);

		EXPECT_THAT(m1, ::testing::ElementsAre(7, 7, 2, 7, 7, 7));

		// todo exception
	}
}

TEST(DMatrix_OperatorTests, T_003_SubtractOperator)
{
	using namespace PrimMatrix;

	{
		using testType = int;

		const DMatrix<testType> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<testType> m2{ 2,3,{6,5,4,3,2,1} };

		const DMatrix<testType> subtractionResult = m1 - m2;

		EXPECT_EQ(subtractionResult.rows(), 2);
		EXPECT_EQ(subtractionResult.columns(), 3);
		EXPECT_EQ(subtractionResult.size(), 6);

		EXPECT_THAT(subtractionResult, ::testing::ElementsAre(-5, -3, -3, 1, 3, 5));

		// todo exception

	}
}

TEST(DMatrix_OperatorTests, T_003_SubtractEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using testType = int;

		DMatrix<testType> m1{ 2,3,{1,2,1,4,5,6} };
		const DMatrix<testType> m2{ 2,3,{6,5,4,3,2,1} };

		m1 -= m2;

		EXPECT_EQ(m1.rows(), 2);
		EXPECT_EQ(m1.columns(), 3);
		EXPECT_EQ(m1.size(), 6);

		EXPECT_THAT(m1, ::testing::ElementsAre(-5, -3, -3, 1, 3, 5));

		// todo exception

	}
}


TEST(DMatrix_OperatorTests, T_004_MultiplicationOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		TestData<test_type> testData(2, 3);
		const DMatrix<test_type> m1{ testData.rows, testData.columns, {1,2,3,4,5,6} };
		const DMatrix<test_type> m2{ testData.columns, testData.rows, {1,2,3,4,5,6} };

		const DMatrix<test_type> multiplicationResult = m1 * m2;

		EXPECT_EQ(multiplicationResult.rows(), testData.rows);
		EXPECT_EQ(multiplicationResult.columns(), testData.rows);
		EXPECT_EQ(multiplicationResult.size(), testData.rows * testData.rows);

		EXPECT_THAT(multiplicationResult, ::testing::ElementsAre(22, 28, 49, 64));

		// todo exception

	}
}

TEST(DMatrix_OperatorTests, T_005_MultiplicationEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		TestData<test_type> testData(2, 3);
		DMatrix<test_type> m1{ testData.rows, testData.columns, {1,2,3,4,5,6} };
		const DMatrix<test_type> m2{ testData.columns, testData.rows, {1,2,3,4,5,6} };

		m1 *= m2;

		EXPECT_EQ(m1.rows(), testData.rows);
		EXPECT_EQ(m1.columns(), testData.rows);
		EXPECT_EQ(m1.size(), testData.rows * testData.rows);

		EXPECT_THAT(m1, ::testing::ElementsAre(22, 28, 49, 64));

		// todo exception

	}
}

TEST(DMatrix_OperatorTests, T_006_ScalarMultiplicationOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		TestData<test_type> testData(2, 3);
		const DMatrix<test_type> matrix{ testData.rows, testData.columns, {1,2,3,4,5,6} };

		const DMatrix<test_type> scalarMulResult = matrix * 3;

		EXPECT_THAT(scalarMulResult, ::testing::ElementsAre(3, 6, 9, 12, 15, 18));
	}
}

TEST(DMatrix_OperatorTests, T_007_ScalarMultiplicationEqualsOperator)
{
	using namespace PrimMatrix;

	{
		using test_type = int;

		TestData<test_type> testData(2, 3);
		DMatrix<test_type> matrix{ testData.rows, testData.columns, {1,2,3,4,5,6} };

		matrix *= 3;

		EXPECT_THAT(matrix, ::testing::ElementsAre(3, 6, 9, 12, 15, 18));
	}
}

TEST(TT, TT)
{
	using namespace PrimMatrix;

	{
		using test_type = int;
		const TestData<test_type> testData{ 5, 2 };

		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns, vec);
		const DMatrix<test_type> transposed = matrix.transpose();

		EXPECT_EQ(transposed.rows(), testData.columns);
		EXPECT_EQ(transposed.columns(), testData.rows);
		EXPECT_EQ(transposed.size(), testData.size);

		EXPECT_THAT(transposed, ::testing::ElementsAre(1, 3, 5, 7, 9, 2, 4, 6, 8, 10));
	}

	{
		using test_type = int;
		const TestData<test_type> testData{ 2, 5 };

		const std::vector<test_type> vec{ 1,2,3,4,5,6,7,8,9,10 };

		const DMatrix<test_type> matrix(testData.rows, testData.columns, vec);
		const DMatrix<test_type> transposed = matrix.transpose();

		EXPECT_EQ(transposed.rows(), testData.columns);
		EXPECT_EQ(transposed.columns(), testData.rows);
		EXPECT_EQ(transposed.size(), testData.size);

		EXPECT_THAT(transposed, ::testing::ElementsAre(1, 6, 2, 7, 3, 8, 4, 9, 5, 10));
	}
}