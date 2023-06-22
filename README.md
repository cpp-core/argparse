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

The `argFlag` methods construct options that expect no arguments and
store an internal state of type `bool`. The methods all requires a
single character template parameter for the short name and `long_name`
and `description` arguments. Some methods also accept a functor that
is invoked each time the option is recognized during parsing.

The following snippet describes an option that can be invoked with
either `-v` or `--verbose` and the presence of the option can be
retrieved by invoking `get`.

```c++
    ArgParse opts(argFlag<'v'>("verbose", "Verbose diagnostics"));
	opts.parse(argc, argv);
	auto verbose = opts.get<'v'>();
```

#### argValue

The `argValue` methods construct options that expect exactly one
argument. The methods all require a single character template
parameter for the short name and arguments for the long name and
description.

Some of the methods also take a defalt value which can be used to
give an initial value to the option. If a default value is not
specified, then the type must be specified as the second template
parameter and the value of the option will be default constructed.

Some of the methods also accept a functor that is invoked each time
the option is recognized during parsing.

#### argValues

The `argValues` methods construct an option that expects zero or more
arguments.

### Parsing Arguments

### Getting Parsed Values

