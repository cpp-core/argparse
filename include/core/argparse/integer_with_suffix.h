// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#pragma once
#include <charconv>
#include "core/lexical_cast/integral.h"
#include "core/lexical_cast/error.h"

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
	try {
	    T value{};
	    int base{10};
	    
	    const char *begin = s.begin();
	    if (s.size() > 1 and s[0] == '0') {
		if (s[1] == 'x' or s[1] == 'X') {
		    begin += 2;
		    base = 16;
		}
		else if (s[1] == 'b' or s[1] == 'B') {
		    begin += 2;
		    base = 2;
		}
		else {
		    begin += 1;
		    base = 8;
		}
	    }
	    
	    auto r = std::from_chars(begin, s.end(), value, base);
	    if (r.ptr == s.end()) return value;
	    else if (r.ptr + 1 == s.end()) {
		if (*r.ptr == 'k') return 1000 * value;
		else if (*r.ptr == 'K') return 1024 * value;
		else if (*r.ptr == 'm') return 1'000'000 * value;
		else if (*r.ptr == 'M') return 1024 * 1024 * value;
		else if (*r.ptr == 'g') return 1'000'000'000 * value;
		else if (*r.ptr == 'G') return 1024 * 1024 * 1024 * value;
	    }
	    throw lexical_cast_error(s, "IntegerWithSuffix");
	} catch (std::invalid_argument const&) {
	    throw lexical_cast_error(s, "IntegerWithSuffix");
	} catch (std::out_of_range const&) {
	    throw lexical_cast_error(s, "IntegerWithSuffix");
	}
    }

    std::string to_string(IntegerWithSuffix<T> value) const {
	return lexical_to_string((T)value);
    }
};

};
