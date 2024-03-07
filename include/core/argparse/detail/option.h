// Copyright (C) 2019, 2021, 2022, 2023, 2024 by Mark Melton
//

#pragma once
#include <set>
#include "base.h"
#include "context.h"
#include "error.h"
#include "core/lexical_cast/builtin.h"
#include "core/lexical_cast/string.h"
#include "core/lexical_cast/optional.h"
#include "core/lexical_cast/error.h"
#include "core/mp/type_name.h"

namespace core::argp
{

struct noop
{
    template<class... Ts>
    constexpr void operator()(Ts&...) const noexcept { }
};

template<char C>
struct ArgBase
{
    static constexpr char FlagCharacter = C;

    ArgBase(std::string_view arg_long_name, std::string_view arg_description)
	: long_name(arg_long_name)
	, description(arg_description)
    { }

    bool matches(std::string_view token) const
    {
	if (token.size() == 2 and token[0] == OptionSymbol)
	    return token[1] == FlagCharacter;
	if (token.size() == long_name.size() + 2
	    and token[0] == OptionSymbol
	    and token[1] == OptionSymbol)
	    return token.substr(2) == long_name;
	return false;
    }

    std::string long_name;
    std::string description;
    std::string value_spec;
    size_t count{0};
};

/// Describes an argument with no parameters.
///
/// \tparam C The single character version of the argument name.
/// \tparam F The optional functor.
/// 
template<char C, class F>
struct ArgFlag : ArgBase<C>
{
    /// Construct an ArgFlag
    ///
    /// \note Use the free function argFlag to construct an ArgFlag.
    /// \param long_name The long version of the argument name.
    /// \param description A description of the argument.
    /// \param func Functor applied when argument is recognized during parsing.
    ArgFlag(std::string_view long_name, std::string_view description, F&& func)
	: ArgBase<C>(long_name, description)
	, function(std::move(func))
    { }

    void match(std::string_view token, Context& ctx)
    {
	++this->count;
	value = true;
	function();
    }
    
    bool value{false};
    F function;
};

/// Construct an ArgFlag describing an argument with no parameters.
///
/// \tparam C The single character version of the argument name.
/// \param long_name The long version of the argument name.
/// \param description A description of the argument.
template<char C>
auto argFlag(std::string_view long_name, std::string_view description)
{ return ArgFlag<C,noop>(long_name, description, std::move(noop{})); }

/// Construct an ArgFlag describing an argument with no parameters.
///
/// \tparam C The single character version of the argument name.
/// \tparam F Functor
/// \param long_name The long version of the argument name.
/// \param description A description of the argument.
/// \param func Functor applied when argument is recognized during parsing.
template<char C, class F>
auto argFlag(std::string_view long_name, std::string_view description, F&& func = noop{})
{ return ArgFlag<C,F>(long_name, description, std::move(func)); }

/// Describes an argument that takes exactly one parameter.
///
/// \tparam C The single character version of the argument name.
/// \tparam T The parameter type.
/// \tparam F The functor type.
template<char C, class T, class F>
struct ArgValue : ArgBase<C>
{
    using Base = ArgBase<C>;

    /// Construct an ArgValue
    ///
    /// \note Use the free functions argValue to construct an ArgValue.
    /// \param long_name The long version of the argument name.
    /// \param default_value The default value of the parameter.
    /// \param description A description of the argument.
    /// \param func Functor applied when argument is recognized during parsing.
    ArgValue(std::string_view long_name, T default_value, std::string_view description, F&& func)
	: Base(long_name, description)
	, value(default_value)
	, function(std::move(func))
    {
	if (Base::FlagCharacter == '*') Base::value_spec = make_spec(long_name, 1, 1);
	else Base::value_spec = make_spec(core::mp::type_name<T>(), 1, 1);
    }
    
    void match(std::string_view token, Context& ctx)
    {
	if (ctx.end() or is_option(ctx.front()))
	    throw missing_value_error(token, ctx, typeid(T));
	
	try { value = core::lexical_cast<T>(ctx.front()); }
	catch (const core::lexical_cast_error& error)
	{ throw bad_value_error(token, ctx, typeid(T)); }

	ctx.pop();
	function(value);
	++Base::count;
    }

    T value;
    F function;
};

/// Construct an ArgValue descibing an argument that takes exactly one paramter.
///
/// \tparam C The single character version of the argument name.
/// \tparam T The type of the parameter.
/// \param long_name The long version of the argument name.
/// \param description A description of the argument.
template<char C, class T>
auto argValue(std::string_view long_name, std::string_view description)
{ return ArgValue<C,T,noop>(long_name, T{}, description, std::move(noop{})); }

/// Construct an ArgValue descibing an argument that takes exactly one paramter.
///
/// \tparam C The single character version of the argument name.
/// \tparam T The type of the parameter.
/// \param long_name The long version of the argument name.
/// \param default_value The default value of the parameter.
/// \param description A description of the argument.
template<char C, class T>
auto argValue(std::string_view long_name, T default_value, std::string_view description)
{ return ArgValue<C,T,noop>(long_name, default_value, description, std::move(noop{})); }

/// Construct an ArgValue descibing an argument that takes exactly one paramter.
///
/// \tparam C The single character version of the argument name.
/// \tparam T The type of the parameter.
/// \param long_name The long version of the argument name.
/// \param description A description of the argument.
/// \param func The functor to apply when the argument is recognized during parsing.
template<char C, class T, class F>
auto argValue(std::string_view long_name, std::string_view description, F&& func)
{ return ArgValue<C,T,F>(long_name, T{}, description, std::move(func)); }

/// Construct an ArgValue descibing an argument that takes exactly one paramter.
///
/// \tparam C The single character version of the argument name.
/// \tparam T The type of the parameter.
/// \param long_name The long version of the argument name.
/// \param default_value The default value of the parameter.
/// \param description A description of the argument.
/// \param func The functor to apply when the argument is recognized during parsing.
template<char C, class T, class F>
auto argValue(std::string_view long_name, T default_value, std::string_view description, F&& func)
{ return ArgValue<C,T,F>(long_name, default_value, description, std::move(func)); }

template<class Container, class T>
void emplace(Container& container, T&& value)
{
    if constexpr (std::is_same_v<Container, std::set<T>>)
		     container.emplace(std::forward<T>(value));
    else
	container.emplace_back(std::forward<T>(value));
}

template<char C, template<class...> class Container, class T, class F>
struct ArgValues : ArgBase<C>
{
    using Base = ArgBase<C>;
    ArgValues(std::string_view long_name, std::string_view description, size_t amin, size_t amax, F&& func)
	: Base(long_name, description)
	, min(amin)
	, max(amax)
	, function(std::move(func))
    {
	if (Base::FlagCharacter == '*') Base::value_spec = make_spec(long_name, min, max);
	else Base::value_spec = make_spec(core::mp::type_name<T>(), min, max);
    }
    
    void match(std::string_view token, Context& ctx)
    {
	while (not ctx.end() and
	       not is_option(ctx.front()) and
	       not is_option_separator(ctx.front()))
	{
	    try
	    {
		auto&& v = core::lexical_cast<T>(ctx.front());
		function(v);
		emplace(value, std::forward<T>(v));
	    }
	    catch (const core::lexical_cast_error& error)
	    { throw bad_value_error(token, ctx, typeid(T)); }

	    ctx.pop();
	}

	auto count = value.size();
	if (count < min)
	    throw too_few_values_error(Base::long_name, ctx, typeid(T), count, min);
	else if (count > max)
	    throw too_many_values_error(Base::long_name, ctx, typeid(T), count, max);
    }

    size_t min, max;
    Container<T> value;
    F function;
};

template<char C, template<class...> class Container, class T>
auto argValues(std::string_view long_name, std::string_view description,
	       size_t min = 1, size_t max = std::numeric_limits<size_t>::max())
{ return ArgValues<C,Container,T,noop>(long_name, description, min, max, std::move(noop{})); }

template<char C, template<class...> class Container, class T, class F>
auto argValuesApply(std::string_view long_name, std::string_view description, F&& func,
		    size_t min = 1, size_t max = std::numeric_limits<size_t>::max())
{ return ArgValues<C,Container,T,F>(long_name, description, min, max, std::move(func)); }

}; // core::argp
