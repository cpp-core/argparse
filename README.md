[![Build](https://github.com/cpp-core/argparse/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/argparse/actions/workflows/build.yaml)

# Ergomomic, Type-Safe Argument Parsing

## Motivation

Parsing command line arguments is a ubiqitous task, but unlike other
modern languages, `C++` has no standard library support for it. The
`argparse` library is a light-weight library that provides ergonomic,
type-safe support for parsing command line arguments. The `argparse`
library supports both builtin and standard library types (such as
`std::vector`) out of the box and is easily extensible to user-defined
types by leveraging the `lexical_cast` library.

The following code snippet is a brief demonstration.

```c++
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

    cout << "data:" << endl;
	for (auto elem : data)
		cout << " " << elem;
    return 0;
}
```

```
> argparse1 --help
program: argparse1 [options]
    -d, --data int [int [...]]           User data
    -v, --verbose                        Verbose diagnostics
	
> argparse1 -d 1 2 3 -v
verbose option selected
data: 1 2 3

```

## Tutorial

The `argparse` library expresses the following opinions about command
line options:
- options are introduced in short form by a single dash, as in `-v`
- options are introduced in long from by two dashes, as in `--verbose`
- options can be combined in short form, as in `-xvz`
- options take zero, one or more arguments

Using `argparse` has three steps:
- Construct an `ArgParse` object describing the options
- Parse the arguments by invoking `parse` or `parse_catch`
- Use the `get` accessor to fetch the parsed values.

### Describing Options

The `argparse` library provides three option flavors: zero argument,
exactly one argument, and zero or more arguments. These options are
constructed using the `argFlag`, `argValue` and `argValues` template
functions respectively.

The first template parameter for each of these template functions is a
non-type parameter of type `char` that represents the single character
name of the option. A template parameter is used rather that a
function argument to make compile-time checking more robust. Two
options with the same single character name or invoking `get` with a
non-existent option will be detected a compile-time and result in a
compilation error.

#### argFlag

The `argFlag` methods are used to construct an option that expect no
arguments and stores an internal state of type `bool`. The most
general `argFlag` method accepts the long name of the argument, a
description and a functor that is invoked each time the option is
encountered while parsing. The functor is optional.

```c++
template<char C, class F>
auto argFlag(std::string_view long_name, std::string_view description, F&& func = noop{});
```

The following snippet describes an option that can be invoked with
either `-v` or `--verbose`, the presence of the option can be
retrieved by invoking `get` and the number of occurrences can be
retrieved by invoking `get_count`.

```c++
    int value{}
    ArgParse opts(argFlag<'v'>("verbose", "Verbose diagnostics", [&]() { value = 42; } ));
    opts.parse(argc, argv);
    auto verbose = opts.get<'v'>();
    auto verbose_count = opts.get_count<'v'>();
```

See the file [`argparse_flag.cpp`](src/tools/argparse_flag.cpp) for a complete example.

#### argValue

The `argValue` methods are used to construct an option that expects
exactly one argument. The most general method accepts the long name of
the argument, a default value, a description and a functor that is
invoked with the option value each time the option is encountered
while parsing. The default value an functor are optional.

```c++
template<char C, class T, class F>
auto argValue(std::string_view long_name, T default_value, std::string_view description, F&& func);
```

The following snippet describes an option that can be invoked with
either `-d` or `--data`, the option value can be retrieved by invoking
`get` and the number of occurrences can be retrieved by invoking
`get_count`.

```c++
    int value{1};
    ArgParse opts
	(
		argValue<'d'>("data", 42, "Data", [&](auto x) { value = 2 * x; })
	 );
    opts.parse(argc, argv);
    auto data = opts.get<'d'>();
    auto data_count = opts.get_count<'d'>();
```

See the file [`argparse_value.cpp`](src/tools/argparse_value.cpp) for a complete example.

#### argValues

See the file [`argparse_values.cpp`](src/tools/argparse_values.cpp) for a complete example.

### Parsing Arguments

### Getting Parsed Values

