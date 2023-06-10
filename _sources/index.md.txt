
# A Timer for Modern C++

{cpp:type}`core::timer::Timer` is a header-only timer for modern `C++`. The
timer is designed for minimal-overhead, ad-hoc timing of `C++` code
including micro-timing down to single machine instructions. The timer
is not designed for benchmarking
([benchmark](https://github.com/google/benchmark) or
[nanobench](https://github.com/martinus/nanobench) are excellent tools
for this purpose) nor for detailed performance analysis.

# Brief Tour

The timer has two basic modes:
- Inovking `run` on isolated code for in vivo measurements.
- Using `start` and `stop` timer calls for in vitro measurements.

## In Vivo Example
The `run` method is designed to repeatedly execute isolated code in
order to measure the average cost per operation. Modern compilers,
taking advantge of the `C++` standard's "as if" rule, will often
eliminate code that produces no visible side-effects making this type
of measurement difficult.

The timer library provides the `core::timer::doNotOptimizeAway`
function which forces the compiler to materialize the given expression
either in memory or in a register. The compiler is still allowed to
fully optimize the expression given that single constraint.

The following example demonstrates using the `run` method coupled with
the `doNotOptimizeAway` function to measure the cost of performing an
integer addition (i.e. `output += 1`). Without the `doNotOptimizeAway`
call, a typical outcome is for the compiler to eliminate all of the
computation since there are no visible side-effects.

```c++
#include <iomanip>
#include <iostream>
#include "core/timer/timer.h"

using namespace core;

int main(int argc, const char *argv[]) {

    unsigned int output{};
    auto ns = timer::Timer().run(1'000'000, [&]() {
	    output += 1;
	    timer::doNotOptimizeAway(output);
    }).elapsed_per_iteration();
    
    std::cout << std::setprecision(2) << ns << " nanoseconds per operation" << std::endl;
    // 0.31 nanoseconds per operation
	
    return 0;
}
```

## In Vitro Example
The `start` and `stop` methods can be used to instrument code as it
exists without isolating it to be executed by `run`. On a modern
platform, each invocation of `start` and `stop` costs tens of clock
cycles, thus, they are designed to be applied to more significant code
segments.

The following example demonstrates using the `start` and `stop`
methods to measure the cost of an exclusive or performed in a
loop. Referencing the `output` variable at the end serves the same
function as the `doNotOptimizeAway` function in the previous example;
it forces the compiler to actually do the computation.

```c++
#include <iostream>
#include "core/timer/timer.h"

using namespace core;

int main(int argc, const char *argv[]) {
    
    timer::Timer timer;
    timer.start();
    unsigned int output{};
    for (auto i = 0; i < 10'000; ++i)
		output ^= i;
    timer.stop();

    auto ns = timer.elapsed().count();
    std::cout << ns << " nanoseconds" << std::endl;
    std::cout << output << std::endl;
    // 3150 nanoseconds
	
    return 0;
}
```

# Installation

```bash
git clone https://github.com/cpp-core/pp
mkdir pp/build && cd pp/build
CC=clang-mp-14 CXX=clang++-mp-14 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
make -j4 check # Run tests
make install   # Do the install
```

To build the documentation (requires doxygen and sphinx-build to be installed)::

```bash
CC=clang-11 CXX=clang++11 cmake -DCORE_DOCS ..
make cxx_core_pp_docs # root of html tree is docs/html/index.html
```

# Background

*Timer* is part of the *cpp-core** family of `C++` libraries. The
*cpp-core* libraries have two primary goals:

- **Ergonomics**. In the spirit of the `CppCoreGuideLines`, cpp-core
  aspires to facilitate writing concise, idiomatic code not by force,
  but by providing a clear path that produces robust, easy to read
  code. Good code should be fun to write.
  
- **Components**. Modern language platforms provide broad support for
  first-class library components, while C++ has traditionally only
  provided a minimal standard library. *cpp-core* aspires to
  provide support for a broad range of libraries that feel like part
  of the language.
  
# License

This software is licensed under the BSD 3-clause license. See the LICENSE file for details.

# Appendix

```{toctree}
---
caption: API REFERENCE
---
timer
```

```{toctree}
caption: FILE REFERENCE
maxdepth: 5
---
include/ <generated/include/dir>
test/ <generated/test/dir>
```
