// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/argparse/argp.h"

using std::cout, std::endl;
using namespace core::argp::interface;

int main(int argc, const char *argv[]) {
    ArgParse opts
	(
	 argValues<'d', std::vector, int>("data", "User data"),
	 argFlag<'v'>("verbose", "Verbose diagnostics")
	 );
    opts.parse(argc, argv);
    auto data = opts.get<'d'>();
    auto verbose = opts.get<'v'>();

    if (verbose) cout << "verbose option selected" << endl;
    else cout << "verbose option not selected" << endl;

    cout << "data" << endl;
    for (auto elem : data)
	cout << elem << endl;
    return 0;
}
