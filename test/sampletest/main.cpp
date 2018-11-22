#include "src/DMatrix.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

#define NDEBUG

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(DMatrix_ConstructorTest, T_001_RowCol)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	const matrixSizeType rows = 10;
	const matrixSizeType columns = 5;
	const matrixSizeType size = rows * columns;

	DMatrix<testType> matrix(rows, columns);

	ASSERT_EQ(matrix.rows(), rows);
	ASSERT_EQ(matrix.columns(), columns);
	ASSERT_EQ(matrix.size(), size);

	// todo
	//std::vector<testType> data(50);
	
}

TEST(DMatrix_ConstructorTest, T_002_RowColInitialValue)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	const matrixSizeType rows = 7;
	const matrixSizeType columns = 3;
	const matrixSizeType size = rows * columns;
	const testType initialValue = 5;

	DMatrix<testType> matrix(rows, columns, initialValue);

	ASSERT_EQ(matrix.rows(), rows);
	ASSERT_EQ(matrix.columns(), columns);
	ASSERT_EQ(matrix.size(), size);

	// todo data
}

TEST(DMatrix_ConstructorTest, T_003_RowColVectorInitialized)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	std::vector<testType> vec{ 1,2,3,4,5,6,7,8,9,10 };

	const matrixSizeType rows = 5;
	const matrixSizeType columns = 2;
	const matrixSizeType size = rows * columns;

	DMatrix<testType> matrix(rows, columns, vec);

	ASSERT_EQ(vec.size(), size);

	ASSERT_EQ(matrix.rows(), rows);
	ASSERT_EQ(matrix.columns(), columns);
	ASSERT_EQ(matrix.size(), size);

	// todo data

}

TEST(DMatrix_ConstructorTest, T_004_VectorVerticalOrientation)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	std::vector<testType> vec{ 1,2,3,4,5,6,7,8,9,10 };

	const matrixSizeType rows = vec.size();
	const matrixSizeType columns = 1;
	const matrixSizeType size = rows * columns;

	DMatrix<testType> matrix(vec, DMatrix<testType>::EOrientation::Vertical);

	ASSERT_EQ(vec.size(), size);

	ASSERT_EQ(matrix.rows(), rows);
	ASSERT_EQ(matrix.columns(), columns);
	ASSERT_EQ(matrix.size(), size);

	// todo data

}

TEST(DMatrix_ConstructorTest, T_004_VectorHorizontalOrientation)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	std::vector<testType> vec{ 1,2,3,4,5,6,7,8,9,10 };

	const matrixSizeType rows = 1;
	const matrixSizeType columns = vec.size();
	const matrixSizeType size = rows * columns;

	DMatrix<testType> matrix(vec, DMatrix<testType>::EOrientation::Horizontal);

	ASSERT_EQ(vec.size(), size);

	ASSERT_EQ(matrix.rows(), rows);
	ASSERT_EQ(matrix.columns(), columns);
	ASSERT_EQ(matrix.size(), size);

	// todo data

}

TEST(TT, TT)
{
	using namespace PrimMatrix;

	using testType = int;
	using matrixSizeType = DMatrix<testType>::size_type;

	std::vector<testType> vec{ 1,2,3,4,5,6,7,8,9,10 };

	const matrixSizeType rows = 5;
	const matrixSizeType columns = 2;
	const matrixSizeType size = rows * columns;

	DMatrix<testType> matrix(rows, columns, vec);

	std::cout << matrix;
	
	DMatrix<testType> transposed = matrix.transpose();
	std::cout << transposed;

	//todo data
}