//#include "src/matrix.h"
#include "gtest/gtest.h"


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(SampleTest, Sample)
{
	EXPECT_EQ(0, 0);
}
