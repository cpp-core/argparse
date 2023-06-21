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

Basic `argparse` usage has three steps:
- Construct an `ArgParse` object containing a description of the
  available options.
- Invoke `parse` or `parse_catch` to parse the given arguments
- Use the `get` accessor to fetch individual values.

### Describing Options

The `argparse` library provides three option flavors: zero argument,
exactly one argument, and zero or more arguments. These options are
constructed using the `argFlag`, `argValue` and `argValues` template
functions respectively.

The first template parameter for each of these template functions is a
non-type parameter of type `char` that represents the single character
name of the option. A template parameter is used rather that a
function argument to make compile-time checking more robust. Two
options with the same single character name or attempting to `get` a
non-existent option will be detected a compile-time and result in a
compilation error.

#### argFlag

The `argFlag` methods construct an option that expects no
arguments. The methods require a `long_name` and `description` for the
option. The following snippet describes an option that can be invoked
with either `-v` or `--verbose`.
the presence of the option can be retrieved with the expression
`opts.get<'v'>()`

```c++
	argFlag<'v'>("--verbose", "Verbose diagnostics");
```

#### argValue

The `argValue` methods construct an option that expects exactly one
argument.

#### argValues

The `argValues` methods construct an option that expects zero or more
arguments.

### Parsing Arguments

### Getting Parsed Values

