// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/argparse/argp.h"

using std::cout, std::endl;
using namespace core::argp::interface;

int main(int argc, const char *argv[]) {
    int a_value{1}, b_value{2};
    ArgParse opts
	(
	 argValue<'a'>("avalue", 42, "Value fo A", [&](auto x) { a_value = 2 * x; }),
	 argValue<'b',size_t>("bvalue", "Value for B", [&](auto x) { b_value = 2 * x; })
	 );
    opts.parse(argc, argv);
    auto a = opts.get<'a'>();
    auto a_count = opts.get_count<'a'>();
    auto b = opts.get<'b'>();
    auto b_count = opts.get_count<'b'>();
    
    cout << "a       : " << a << endl;
    cout << "a count : " << a_count << endl;
    cout << "a value : " << a_value << endl;

    cout << "b       : " << b << endl;
    cout << "b count : " << b_count << endl;
    cout << "b value : " << b_value << endl;
    return 0;
}
