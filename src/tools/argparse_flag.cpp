// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/argparse/argp.h"

using std::cout, std::endl;
using namespace core::argp::interface;

int main(int argc, const char *argv[]) {
    int a{};
    ArgParse opts
	(
	 argFlag<'v'>("verbose", "Verbose diagnostics", [&]() { a += 42; })
	 );
    opts.parse(argc, argv);
    auto verbose = opts.get<'v'>();
    auto verbose_count = opts.get_count<'v'>();
    
    cout << std::boolalpha << endl;
    cout << "flag  : " << verbose << endl;
    cout << "count : " << verbose_count << endl;
    cout << "a     : " << a << endl;
    return 0;
}
