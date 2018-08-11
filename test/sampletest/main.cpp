#include "src/DMatrix.h"
#include "gtest/gtest.h"


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
