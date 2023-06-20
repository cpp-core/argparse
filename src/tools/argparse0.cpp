// Copyright (C) 2023 by Mark Melton
//

#include <charconv>
#include <iostream>
#include "core/argparse/argp.h"
#include "core/lexical_cast/impl.h"

using std::cout, std::endl;
using namespace core::argp::interface;

template<std::integral T>
class IntegerWithSuffix {
public:
    IntegerWithSuffix(T value = {})
	: value_(value) {
    }
    
    operator T() const {
	return value_;
    }

private:
    T value_;
};

namespace core::lexical_cast_detail {
template<class T>
struct lexical_cast_impl<IntegerWithSuffix<T>> {
    IntegerWithSuffix<T> convert(std::string_view s) const {
	T value{};
	auto r = std::from_chars(s.begin(), s.end(), value, 10);
	if (r.ptr == s.end())
	    return value;
	if (*r.ptr == 'k') return 1000 * value;
	else if (*r.ptr == 'K') return 1024 * value;
	else if (*r.ptr == 'm') return 1'000'000 * value;
	else if (*r.ptr == 'M') return 1024 * 1024 * value;
	else if (*r.ptr == 'g') return 1'000'000'000 * value;
	else if (*r.ptr == 'G') return 1024 * 1024 * 1024 * value;
	else throw lexical_cast_error(s, "IntegerWithSuffix");
    }

    std::string to_string(IntegerWithSuffix<T> value) const {
	return lexical_to_string((T)value);
    }
};
};

int main(int argc, const char *argv[]) {
    ArgParse opts
	(
	 argValues<'d', std::vector, int>("data", "User data"),
	 argValue<'n'>("number", IntegerWithSuffix<size_t>{}, "General number"),
	 argFlag<'v'>("verbose", "Verbose diagnostics")
	 );
    opts.parse(argc, argv);
    auto data = opts.get<'d'>();
    auto number = opts.get<'n'>();
    auto verbose = opts.get<'v'>();

    if (verbose) cout << "verbose option selected" << endl;
    else cout << "verbose option not selected" << endl;

    cout << "number: " << number << endl;
    cout << "data:";
    for (auto elem : data)
	cout << " " << elem;
    return 0;
}
