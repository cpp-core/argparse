// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#pragma once
#include "core/lexical_cast/floating.h"
#include "core/lexical_cast/error.h"

template<std::floating_point T>
class FloatingWithSuffix {
public:
    FloatingWithSuffix(T value = {})
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
struct lexical_cast_impl<FloatingWithSuffix<T>> {
    FloatingWithSuffix<T> convert(std::string_view s) const {
	try {
	    T value{};
	    auto r = core::lexical_cast_detail::from_chars(s.begin(), s.end(), value);
	    if (r.ptr == s.end()) return value;
	    else if (r.ptr + 1 == s.end()) {
		if (*r.ptr == 'k') return 1000 * value;
		else if (*r.ptr == 'K') return 1024 * value;
		else if (*r.ptr == 'm') return 1'000'000 * value;
		else if (*r.ptr == 'M') return 1024 * 1024 * value;
		else if (*r.ptr == 'g') return 1'000'000'000 * value;
		else if (*r.ptr == 'G') return 1024 * 1024 * 1024 * value;
	    }
	    throw lexical_cast_error(s, "FloatingWithSuffix");
	} catch (std::invalid_argument const&) {
	    throw lexical_cast_error(s, "FloatingWithSuffix");
	} catch (std::out_of_range const&) {
	    throw lexical_cast_error(s, "FloatingWithSuffix");
	}
    }

    std::string to_string(FloatingWithSuffix<T> value) const {
	return lexical_to_string((T)value);
    }
};

};
