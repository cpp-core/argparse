// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/argparse/argp.h"
#include "core/argparse/integer_with_suffix.h"
#include "core/argparse/floating_with_suffix.h"

using namespace core::argp::interface;
using std::cout, std::endl;

int main(int argc, const char *argv[]) {
    ArgParse opts
	(
	 argValues<'d', std::vector, int>("data", "User data"),
	 argValue<'n'>("number", IntegerWithSuffix<size_t>{}, "General number"),
	 argValue<'f'>("floating", FloatingWithSuffix<double>{}, "Floating number"),
	 argFlag<'v'>("verbose", "Verbose diagnostics")
	 );
    opts.parse(argc, argv);
    auto data = opts.get<'d'>();
    auto number = opts.get<'n'>();
    auto floating = opts.get<'f'>();
    auto verbose = opts.get<'v'>();

    if (verbose) cout << "verbose option selected" << endl;
    else cout << "verbose option not selected" << endl;

    cout << "number: " << number << endl;
    cout << "floating: " << floating << endl;
    cout << "data:";
    for (auto elem : data)
	cout << " " << elem;
    return 0;
}
