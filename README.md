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

    cout << "data" << endl;
	for (auto elem : data)
		cout << elem << endl;
    return 0;
}
```

## Tutorial
