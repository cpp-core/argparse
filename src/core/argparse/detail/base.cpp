// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#include "core/argparse/detail/base.h"

namespace core::argp {

bool is_identifier(char c)
{
    return (c >= 'a' and c <= 'z')
	or (c >= 'A' and c <= 'Z');
}
    
bool is_long_option(std::string_view arg)
{
    return arg.size() > 2
	and arg[0] == OptionSymbol
	and arg[1] == OptionSymbol
	and is_identifier(arg[2]);
}
    
bool is_short_option(std::string_view arg)
{
    return arg.size() == 2
	and arg[0] == OptionSymbol
	and is_identifier(arg[1]);
}

bool is_option_separator(std::string_view arg)
{
    return arg.size() == 2
	and arg[0] == OptionSymbol
	and arg[1] == OptionSymbol;
}

bool is_option(std::string_view arg)
{
    return is_long_option(arg)
	or is_short_option(arg);
}

bool is_option_group(std::string_view arg)
{
    return arg.size() > 2
	and arg[0] == OptionSymbol
	and is_identifier(arg[1])
	and is_identifier(arg[2]);
}

std::string make_spec(std::string_view arg_value, size_t min, size_t max)
{
    std::string value = std::string(arg_value);
    std::string r;
    if (min == 0);
    else if (min == 1) r = value + " ";
    else if (min == 2) r = value + " " + value + " ";
    else if (min > 2) return value + " " + value + " ... ";

    if (max - min == 0);
    else if (max - min == 1) r += "[" + value + "]";
    else if (max - min == 2) r += "[" + value + " [" + value + "]]";
    else r += "[" + value + " [...]]";

    return r;
}

}; // core::argp
