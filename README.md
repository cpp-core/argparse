[![Build](https://github.com/cpp-core/argparse/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/argparse/actions/workflows/build.yaml)

# Ergomomic, Type-Safe Argument Parsing

## Motivation

Parsing command line arguments is a ubiqitous task, but unlike other
modern languages, `C++` has no standard library support. The
`argparse` library is a light-weight library that provides ergonimic,
type-safe support for parsing command line arguments. The `argparse`
library supports standard library types such as `std::vector` and
`std::map` out of the box and is easily extensible to user-defined
types by leveraging the header-only `lexical_cast` library.

The following code snippet demonstrats how to parse a simple flag.

```c++
int main(int argc, const char *argv[]) {
	ArgParse opts(argFlag<'v'>("verbose", "Verbose diagnostics"));
	opts.parse(argc, argv);
	auto verbose = opts.get<'v'>();
	
    if (verbose) cout << "verbose option selected" << endl;
    else cout << "verbose option not selected" << endl;
    return 0;
}
```

## Tutorial
