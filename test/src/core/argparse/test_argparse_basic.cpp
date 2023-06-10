// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#include <fmt/format.h>
#include <gtest/gtest.h>
#include <list>
#include "core/util/argparse/argp.h"
#include "core/util/argparse/message.h"

using namespace core::argp::interface;
namespace argp = core::argp;
using namespace std::string_literals;

TEST(ArgParse, ArgFlag)
{
    ArgParse opts
	(
	 argFlag<'a'>("aflag", "Flag A"),
	 argFlag<'b'>("bflag", "Flag B")
	 );
    opts.parse({"program", "-a"});

    EXPECT_EQ(opts.get<'a'>(), true);
    EXPECT_EQ(opts.get<'b'>(), false);
}

TEST(ArgParse, ArgFlagApply)
{
    int a{0}, b{0};
    ArgParse opts
	(
	 argFlag<'a'>("aflag", "Flag A", [&]() { a = 42; }),
	 argFlag<'b'>("bflag", "Flag B", [&]() { b = 7; })
	 );
    opts.parse({"program", "-a"});

    EXPECT_EQ(opts.get<'a'>(), true);
    EXPECT_EQ(opts.get<'b'>(), false);
    
    EXPECT_EQ(a, 42);
    EXPECT_EQ(b, 0);
}

TEST(ArgParse, ArgFlagCount)
{
    ArgParse opts
	(
	 argFlag<'a'>("aflag", "Flag A"),
	 argFlag<'b'>("bflag", "Flag B")
	 );
    opts.parse({"program", "-a", "-a"});

    EXPECT_EQ(opts.get_count<'a'>(), 2);
    EXPECT_EQ(opts.get_count<'b'>(), 0);
}

TEST(ArgParse, ArgValue)
{
    ArgParse opts
	(
	 argValue<'a'>("aint", 0, "Int A"),
	 argValue<'b'>("bstr", "baz"s, "String B"),
	 argValue<'c',int>("cint", "Int C"),
	 argValue<'d',std::string>("dstr", "String D")
	 );
    opts.parse({"program", "-a", "-10", "-d", "foo"});

    EXPECT_EQ(opts.get<'a'>(), -10);
    EXPECT_EQ(opts.get<'b'>(), "baz");
    EXPECT_EQ(opts.get<'c'>(), 0);
    EXPECT_EQ(opts.get<'d'>(), "foo");
}

TEST(ArgParse, ArgValueApply)
{
    int aint{0}, cint{7};
    std::string bstr{"baz"}, dstr{"baz"};
    ArgParse opts
	(
	 argValue<'a'>("aint", 0, "Int A", [&](auto value) { aint = value; }),
	 argValue<'b'>("bstr", "baz"s, "String B", [&](auto value) { bstr = value; }),
	 argValue<'c',int>("cint", "Int C", [&](auto value) { cint = value; }),
	 argValue<'d',std::string>("dstr", "String D", [&](auto value) { dstr = value; })
	 );
    opts.parse({"program", "-a", "-10", "-d", "foo"});

    EXPECT_EQ(opts.get<'a'>(), -10);
    EXPECT_EQ(aint, -10);
    
    EXPECT_EQ(opts.get<'b'>(), "baz");
    EXPECT_EQ(bstr, "baz");
    
    EXPECT_EQ(opts.get<'c'>(), 0);
    EXPECT_EQ(cint, 7);
    
    EXPECT_EQ(opts.get<'d'>(), "foo");
    EXPECT_EQ(dstr, "foo");
}

TEST(ArgParse, ArgValues)
{
    ArgParse opts
	(
	 argValues<'a', std::vector, int>("aint", "Ints A"),
	 argValues<'b', std::list, std::string>("bstr", "Strings B"),
	 argValues<'c', std::set, int>("cset", "Ints C")
	 );
    opts.parse({"program", "-a", "-10", "7", "-b", "foo", "-c", "1", "2", "3"});

    EXPECT_EQ(opts.get<'a'>(), (std::vector<int>{ -10, 7 }));
    EXPECT_EQ(opts.get<'b'>(), std::list<std::string>{"foo"});
    EXPECT_EQ(opts.get<'c'>(), (std::set<int>{1, 2, 3}));
}

TEST(ArgParse, ThrowUnknownOptionError)
{
    {
	ArgParse opts
	    (
	     argFlag<'a'>("aflag", "Flag A"),
	     argFlag<'b'>("bflag", "Flag B")
	     );
	ASSERT_THROW(opts.parse({"program", "-c"}), argp::unknown_option_error);
    }
    
    ArgParse opts
	(
	 argFlag<'a'>("aflag", "Flag A"),
	 argFlag<'b'>("bflag", "Flag B")
	 );

    try { opts.parse({"program", "-c"}); }
    catch (const argp::unknown_option_error& e)
    { EXPECT_EQ(std::string(e.what()), fmt::format(argp::unknown_option_msg, "-c")); }
}

TEST(ArgParse, ThrowMissingValueError)
{
    {
	ArgParse opts
	    (
	     argValue<'a'>("aint", 0, "Int A"),
	     argValue<'b'>("bstr", "baz"s, "String B")
	     );
	ASSERT_THROW(opts.parse({"program", "-a", "-b", "foo"}), argp::missing_value_error);
    }
    
    ArgParse opts
	(
	 argValue<'a'>("aint", 0, "Int A"),
	 argValue<'b'>("bstr", "baz"s, "String B")
	 );

    try { opts.parse({"program", "-a", "-b", "foo"}); }
    catch (const argp::missing_value_error& e)
    { EXPECT_EQ(std::string(e.what()), fmt::format(argp::missing_value_msg, "-a", "int")); }
}

TEST(ArgParse, ThrowBadValueError)
{
    {
	ArgParse opts
	    (
	     argValue<'a'>("aint", 0, "Int A"),
	     argValue<'b'>("bstr", "baz"s, "String B")
	     );
	ASSERT_THROW(opts.parse({"program", "-a", "abc", "-b", "foo"}), argp::bad_value_error);
    }
    
    ArgParse opts
	(
	 argValue<'a'>("aint", 0, "Int A"),
	 argValue<'b'>("bstr", "baz"s, "String B")
	 );

    try { opts.parse({"program", "-a", "abc", "-b", "foo"}); }
    catch (const argp::bad_value_error& e)
    { EXPECT_EQ(std::string(e.what()), fmt::format(argp::bad_value_msg, "abc", "int", "-a")); }
}

TEST(ArgParse, ThrowTooFewValuesError)
{
    {
	ArgParse opts
	    (
	     argValues<'a', std::vector, int>("aint", "Ints A", 3, 5),
	     argValues<'b', std::list, std::string>("bstr", "Strings B", 0)
	     );
	ASSERT_THROW(opts.parse({"program", "-a", "-10", "7", "-b"}), argp::too_few_values_error);
    }

    ArgParse opts
	(
	 argValues<'a', std::vector, int>("aint", "Ints A", 3, 5),
	 argValues<'b', std::list, std::string>("bstr", "Strings B", 0)
	 );
    try { opts.parse({"program", "-a", "-10", "7", "-b"}); }
    catch (const argp::too_few_values_error& e)
    { EXPECT_EQ(std::string(e.what()), fmt::format(argp::too_few_values_msg, 3, "int", "aint", 2)); }
}

TEST(ArgParse, ThrowTooManyValuesError)
{
    {
	ArgParse opts
	    (
	     argValues<'a', std::vector, int>("aint", "Ints A", 0, 1),
	     argValues<'b', std::list, std::string>("bstr", "Strings B", 0)
	     );
	ASSERT_THROW(opts.parse({"program", "-a", "-10", "7", "-b"}), argp::too_many_values_error);
    }

    ArgParse opts
	(
	 argValues<'a', std::vector, int>("aint", "Ints A", 0, 1),
	 argValues<'b', std::list, std::string>("bstr", "Strings B", 0)
	 );
    try { opts.parse({"program", "-a", "-10", "7", "-b"}); }
    catch (const argp::too_many_values_error& e)
    { EXPECT_EQ(std::string(e.what()), fmt::format(argp::too_many_values_msg, 1, "int", "aint", 2)); }
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
