// Copyright (C) 2019, 2022 by Mark Melton
//

#pragma once
#include <queue>
#include <string>

namespace core::argp
{

static constexpr char OptionSymbol = '-';

bool is_identifier(char c);
bool is_long_option(std::string_view arg);
bool is_short_option(std::string_view arg);
bool is_option_separator(std::string_view arg);
bool is_option(std::string_view arg);
bool is_option_group(std::string_view arg);

std::string make_spec(std::string_view arg_value, size_t min, size_t max);

}; // core::argp
