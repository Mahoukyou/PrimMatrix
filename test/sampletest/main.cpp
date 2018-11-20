#include "src/DMatrix.h"
#include "gtest/gtest.h"

#include <iostream>
#include <vector>

#define NDEBUG

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(SampleTest, Sample)
{
	using namespace PrimMatrix;
	DMatrix<int> d(10, 5);

	ASSERT_EQ(d.rows(), 10);
	ASSERT_EQ(d.columns(), 5);

	ASSERT_EQ(d(0, 0), 0);
}

TEST(SampleTest, Vector)
{
	using namespace PrimMatrix;
	std::vector<int> vec(50);


	DMatrix<int> d(10, 5, vec);

	ASSERT_EQ(d.rows(), 10);
	ASSERT_EQ(d.columns(), 5);

	ASSERT_EQ(d(0, 0), 0);
}

TEST(SampleTest, Vertical)
{
	using namespace PrimMatrix;
	std::vector<int> vec(15);


	DMatrix<int> d(vec, DMatrix<int>::EOrientation::Vertical);

	ASSERT_EQ(d.rows(), 15);
	ASSERT_EQ(d.columns(), 1);

	ASSERT_EQ(d(0, 0), 0);

	std::cout << d;
}
