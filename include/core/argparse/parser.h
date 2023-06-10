// Copyright (C) 2019, 2021, 2022, 2023 by Mark Melton
//

#pragma once
#include <any>
#include <iostream>
#include "base.h"
#include "context.h"
#include "error.h"
#include "core/tuple/find.h"
#include "core/tuple/map.h"
#include "core/tuple/fold.h"
#include "core/mp/constants.h"
#include "core/mp/find_index.h"
#include "core/mp/transform.h"

namespace core::argp {

template<class T>
using flag_character = core::mp::_char<T::FlagCharacter>;

/// Describes a set of command line arguments.
///
/// \tparam Ts ArgFlag, ArgValue or ArgValues
template<class... Ts>
class ArgParse
{
public:
    using Tuple = std::tuple<Ts...>;
    using Flags = core::mp::transform_t<flag_character, core::mp::list<Ts...>>;

    /// Construct a description of a set of command line arguments.
    ///
    /// \param args Descriptions of individual arguments.
    ArgParse(Ts&&... args)
	: m_tuple(std::make_tuple(std::move(args)...))
    { }

    template<char C>
    auto get()
    {
	constexpr auto Idx = core::mp::find_index_v<Flags, core::mp::_char<C>>;
	static_assert(Idx < std::tuple_size_v<Tuple>, "\n\n"
		      "static assertion: No option with the given name exists.\n"
		      "static assertion: Ignore subsequent compiler errors for the next line.\n");
	return std::get<Idx>(m_tuple).value;
    }

    template<char C>
    auto get_count()
    {
	constexpr auto Idx = core::mp::find_index_v<Flags, core::mp::_char<C>>;
	static_assert(Idx < std::tuple_size_v<Tuple>, "\n\n"
		      "static assertion: No option with the given name exists.\n"
		      "static assertion: Ignore subsequent compiler errors for the next line.\n");
	return std::get<Idx>(m_tuple).count;
    }

    void process_token(std::string_view token, Context& ctx)
    {
	using core::tp::find_first;
	using core::tp::map_nth;
		
	auto idx = find_first(m_tuple, [&](const auto& e) { return e.matches(token); });

	if (idx < 0)
	{
	    if (token == "-*") throw unknown_option_error(ctx.front(), ctx);
	    else throw unknown_option_error(token, ctx);
	}
	
	map_nth([&](auto& e) { e.match(token, ctx); }, idx, m_tuple);
    }

    std::string star_value_spec()
    {
	auto printer = [&](const auto& result, const auto& arg)
		       {
			   if (arg.FlagCharacter != '*')
			       return result;
			   return result + " " + arg.value_spec;
		       };
	return core::tp::fold_l(printer, std::string(), m_tuple);
    }
    
    void output_help_message(std::ostream& os, const std::vector<std::string>& args)
    {
	auto program_name = args.size() > 0 ? args[0] : "unknown program";
	std::cout << "program: " << program_name << " [options]" << star_value_spec() << std::endl;
	auto printer = [&](const auto& arg)
		       {
			   static constexpr size_t Indent = 4;
			   static constexpr size_t Align = 10;
			   static constexpr size_t Info = 30;
			   if (arg.FlagCharacter == '*')
			       return;

			   auto len = arg.long_name.size() + arg.value_spec.size();
			   auto n = Align - len % Align;
			   if (n + len < Info)
			       n = Info - len;

			   os << std::string(Indent, ' ');
			   os << "-" << arg.FlagCharacter << ", ";
			   os << "--" << arg.long_name << " ";
			   os << arg.value_spec;
			   os << std::string(n, ' ');
			   os << arg.description << std::endl;
		       };
	core::tp::map_inplace(printer, m_tuple);
    }

    bool parse(const std::vector<std::string>& args)
    {
	Context ctx{args};
	ctx.pop();
	
	bool done_with_options{false};
	while (not ctx.end())
	{
	    const auto& token = ctx.front();
	    if (done_with_options or token[0] != OptionSymbol)
	    {
		process_token("-*", ctx);
	    }
	    else if (ctx.front() == "--help")
	    {
		output_help_message(std::cout, args);
		exit(0);
	    }
	    else if (is_option_separator(token))
	    {
		ctx.pop();
		done_with_options = true;
	    }
	    else if (is_option(token))
	    {
		ctx.pop();
		process_token(token, ctx);
	    }
	    else if (is_option_group(token))
	    {
		ctx.pop();
		for (auto c : token.substr(1))
		    process_token(std::string{'-', c}, ctx);
	    }
	    else
		throw unknown_option_error(token, ctx);
	}
	
	return true;
    }

    /// Parse the arguments.
    ///
    /// \param largs Arguments
    /// \returns True if arguments are parsed successfully.
    bool parse(std::initializer_list<std::string> largs)
    {
	std::vector<std::string> args(largs.begin(), largs.end());
	return parse(args);
    }
    
    /// Parse the command line arguments.
    ///
    /// \param argc Argument count (unix conventions).
    /// \param argv Array of pointers to arguments.
    /// \returns True if arguments are parsed successfully.
    bool parse(int argc, const char *argv[])
    {
	std::vector<std::string> args;
	for (size_t i = 0; i < size_t(argc); ++i)
	    args.emplace_back(std::string(argv[i]));
	return parse(args);
    }
    
private:
    Tuple m_tuple;
    std::vector<std::string> m_extra;
};

}; // core::argp


