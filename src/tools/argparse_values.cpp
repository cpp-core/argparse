// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/argparse/argp.h"

using std::cout, std::endl;
using namespace core::argp::interface;

int main(int argc, const char *argv[]) {
    ArgParse opts
	(
	 argValues<'d', std::vector, int>("data", "Data"),
	 argValues<'*', std::vector, std::string>("files", "Files")
	 );
    opts.parse(argc, argv);
    auto data = opts.get<'d'>();
    const auto& files = opts.get<'*'>();
    
    cout << "data  :";
    for (auto elem : data)
	cout << " " << elem;
    cout << endl;

    cout << "file  :";
    for (const auto& file : files)
	cout << " " << file;
    cout << endl;
    
    return 0;
}
