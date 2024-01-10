# What is this?

`integral_types` is a small, single-header library for C++11 or greater providing fixed-width integer types, not unlike [`<cstdint>`](https://en.cppreference.com/w/cpp/types/integer) from the standard library. The main distinguishing feature is that `integral_types` features the number of bits as a non-type template parameter instead of encoding it in the name. For example, `std::int8_t` becomes `integral_types::int_exact_t<8>`.

`integral_types` should work on any valid implementation.

# Why?

I occasionally find myself writing personal projects that would benefit from being able to select the smallest necessary integer at compile time, and I was tired of rewriting essentially this exact header file every time.

# Documentation
### _Note:_
Under the hood, this library currently works by selecting from the [fundamental integer types](https://en.cppreference.com/w/cpp/language/types) -- `signed` and `unsigned` versions of `char`, `short`, `int`, `long`, and `long long`. It does not currently provide a wrapper for implementation-specific/extended integer types.

## Getting `integral_types`
There are currently two ways add this library to a project. First, it is simple enough as a single-header library with no dependencies to copy it from `src/integral_types/integral_types.hpp` into wherever would be most convenient.

Second, if using `cmake`:
```cmake
Include(FetchContent)
FetchContent_Declare(
    integral_types
    GIT_REPOSITORY https://github.com/robotfield/integral_types.git
    GIT_TAG 1.0.0
)
```
> ...
```cmake
FetchContent_MakeAvailable(integral_types)
```
> ...
```cmake
target_link_libraries([your target here] INTERFACE integral_types)
```
Then, in your C++ files, you should be able to use `integral_types` like so:
```c++
#include <iostream>
#include <integral_types/integral_types.hpp>

int main() {
    integral_types::int_least_t<42> hello_world = 6 * 9;
    std::cout << hello_world << std::endl;
    return 0;
}
```

## `integral_types::bit_sizeof`
An alias for `std::integral_constant<std::size_t, CHAR_BIT * sizeof(T)>` for those who avoid macros just for the sake of it.
```c++
static_assert(integral_types::bit_sizeof<char>::value >= 8, "true");
static_assert(integral_types::bit_sizeof_v<char> >= 8, "true");  // (C++14 and up)
```

## `integral_types::int_exact_t`, `integral_types::uint_exact_t`
Analogous to `std::intN_t` and `std::uintN_t`. Instantiating one with an unavailable number of bits will `static_assert(N && false, "...")`.

```c++
// Signed, 8-bit int, fine on POSIX-compliant machines
integral_types::int_exact_t<8> small = 127;

// The unsigned version
integral_types::uint_exact_t<8> bigger = 255;

// Compiler error in anything running C/C++ code
integral_types::int_exact_t<7> smaller = 63; 
```

## `integral_types::int_least_t`, `integral_types::uint_least_t`
Analogous to `std::int_leastN_t` and `std::uint_leastN_t`. Instantiation yields the first `signed` or `unsigned` `char`, `short`, `int`, `long`, or `long long` to be greater than or equal `N`. If none satisfy that requirement, `static_assert(N && false, "...")`. The default value of `N` is `0`.

```c++
integral_types::int_least_t x; // Char
integral_types::int_least_t<7> y; // Char
integral_types::uint_least_t<8> z; // Unsigned char
integral_types::int_least_t<9> w; // Not char (POSIX)
```

## `integral_types::int_fit_t`, `integral_types::uint_fit_t`
The first (`signed` or `unsigned`, respectively) `long long`, `long`, `int`, `short`, or `char` to have a `bit_sizeof` less than or equal to that of `std::size_t`. It's intended to be an integral type that 'fits' on the processor at least somewhat conveniently. It takes no template parameters.

```c++
// Should print 64 on a system with 64-bit addresses.
std::cout << integral_types::bit_sizeof<integral_type::int_fit_t>::value << std::endl;
```

# Testing
To test `integral_types` in your environment, clone this `git` repository into a local directory.
```bash
$ git clone https://github.com/robotfield/integral_types.git
$ cd integral_types
```
If you use `gcc`/`g++` or `clang`, then run the following command:
```bash
$ cmake --preset=dev-default -DCMAKE_BUILD_TYPE=DEBUG
```
Otherwise, if you use `msvc`, then run this instead:
```bash
$ cmake --preset=dev-msvc -DCMAKE_BUILD_TYPE=DEBUG
```
Next (regardless of which of the previous two commands you just entered), run the following:
```bash
$ cd build
$ make
```
You may also run `make test` after this, but all of the tests for this library are written as somewhat paranoid `static_assert`s split across 5 files with bare `main()` functions, so the real evaluation is performed at compile time.
