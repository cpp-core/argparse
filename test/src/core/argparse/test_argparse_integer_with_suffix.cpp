// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#include <fmt/format.h>
#include <gtest/gtest.h>
#include "core/argparse/argp.h"
#include "core/argparse/integer_with_suffix.h"
#include "core/mp/foreach.h"

using namespace core::argp::interface;

TEST(ArgParse, IntegerWithSuffix)
{
    auto test = []<class T>() {
	IntegerWithSuffix<T> value{3};
	EXPECT_EQ(value, 3);
	EXPECT_EQ(core::lexical_to_string(value), "3");
	
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3"), 3);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("030"), 24);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("0x30"), 48);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("0X30"), 48);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("0b11"), 3);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("0B11"), 3);
	
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3k"), 3 * 1'000);
	EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3K"), 3 * 1'024);

	if constexpr (sizeof(T) > 2) {
	    EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3m"), 3 * T{1'000'000});
	    EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3M"), 3 * 1024 * T{1024});
	}
	
	if constexpr (sizeof(T) > 4) {
	    EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3g"), 3 * T{1'000'000'000});
	    EXPECT_EQ(core::lexical_cast<IntegerWithSuffix<T>>("3G"), 3 * 1024 * 1024 * T{1024});
	}
    };
    core::mp::foreach<int, unsigned int, long, unsigned long, long long, unsigned long long>(test);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
