// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#include <fmt/format.h>
#include <gtest/gtest.h>
#include "core/argparse/argp.h"
#include "core/argparse/floating_with_suffix.h"
#include "core/mp/foreach.h"

using namespace core::argp::interface;

TEST(ArgParse, FloatingWithSuffix)
{
    auto test = []<class T>() {
	FloatingWithSuffix<T> value{3};
	EXPECT_EQ(value, 3);
	EXPECT_EQ(core::lexical_to_string(value), "3.000000");
	
	EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3"), 3);
	
	EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3k"), 3 * 1'000);
	EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3K"), 3 * 1'024);

	if constexpr (sizeof(T) > 2) {
	    EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3m"), 3 * T{1'000'000});
	    EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3M"), 3 * 1024 * T{1024});
	}
	
	if constexpr (sizeof(T) > 4) {
	    EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3g"), 3 * T{1'000'000'000});
	    EXPECT_EQ(core::lexical_cast<FloatingWithSuffix<T>>("3G"), 3 * 1024 * 1024 * T{1024});
	}
    };
    core::mp::foreach<float, double, long double>(test);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
