#include <climits>
#include <cstdint>
#include <type_traits>

#include <integral_types/integral_types.hpp>

using namespace std;
using namespace integral_types;

// Check that integral_types made to be the same size as whatever variadic
// arguments fed to the template are actually integral types
template <template <size_t> class, typename...>
struct always_integral;
template <template <size_t> class Int_t>
struct always_integral<Int_t> {
  static constexpr bool value = true;
};
template <template <size_t> class Int_t, typename T, typename... Ts>
struct always_integral<Int_t, T, Ts...> {
  static constexpr bool
    value = std::is_integral<Int_t<bit_sizeof<T>::value>>::value
         && always_integral<Int_t, Ts...>::value;
};

// Same as previous, but instead of generating the integral_types from the sizes
// of supplied variadic arguments, generate them by counting down from the
// supplied template size_t
template <template <size_t> class Int_t, size_t bits>
struct always_integral_count {
  static constexpr bool value = std::is_integral<Int_t<bits>>::value
                             && always_integral_count<Int_t, bits - 1>::value;
};
template <template <size_t> class Int_t>
struct always_integral_count<Int_t, 0> {
  static constexpr bool value = std::is_integral<Int_t<0>>::value;
};

// Check that integral_types generated to be the same size as integral types fed
// to the variadic template actually are the same size
template <template <size_t> class, typename...>
struct same_size_invariant;
template <template <size_t> class Int_t>
struct same_size_invariant<Int_t> {
  static constexpr bool value = true;
};
template <template <size_t> class Int_t, typename T, typename... Ts>
struct same_size_invariant<Int_t, T, Ts...> {
  static constexpr bool value = sizeof(Int_t<bit_sizeof<T>::value>) == sizeof(T)
                             && same_size_invariant<Int_t, Ts...>::value;
};

static_assert(
  bit_sizeof<char>::value == CHAR_BIT,
  "bit_sizeof failed cxx_std_11 compilation check");
static_assert(
  bit_sizeof<size_t>::value == sizeof(size_t) * CHAR_BIT,
  "bit_sizeof failed cxx_std_11 compilation check");

static_assert(
  always_integral<
    int_exact_t,
    signed char,
    signed short,
    signed int,
    signed long,
    signed long long>::value,
  "int_t failed cxx_std_11 compilation check -- not an integral type");
static_assert(
  always_integral<
    uint_exact_t,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long>::value,
  "uint_t failed cxx_std_11 compilation check -- not an integral type");

static_assert(
  always_integral_count<int_least_t, bit_sizeof<size_t>::value>::value,
  "int_least_t failed cxx_std_11 compilation check -- not an integral type");
static_assert(
  always_integral_count<uint_least_t, bit_sizeof<size_t>::value>::value,
  "uint_least_t failed cxx_std_11 compilation check -- not an integral type");

static_assert(
  same_size_invariant<
    int_exact_t,
    signed char,
    signed short,
    signed int,
    signed long,
    signed long long>::value,
  "int_t failed cxx_std_11 compilation check -- bit size of available integral "
  "type does not yield a type of the same bit size");
static_assert(
  same_size_invariant<
    uint_exact_t,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long>::value,
  "uint_t failed cxx_std_11 compilation check -- bit size of available "
  "integral type does not yield a type of the same bit size");
static_assert(
  same_size_invariant<
    int_least_t,
    signed char,
    signed short,
    signed int,
    signed long,
    signed long long>::value,
  "int_least_t failed cxx_std_11 compilation check -- bit size of available "
  "integral type does not yield a type of the same bit size");
static_assert(
  same_size_invariant<
    uint_least_t,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long>::value,
  "uint_least_t failed cxx_std_11 compilation check -- bit size of available "
  "integral type does not yield a type of the same bit size");

static_assert(
  sizeof(int_fit_t) <= sizeof(size_t) || sizeof(uint_fit_t) <= sizeof(size_t),
  "int_fit_t/uint_fit_t failed cxx_std_11 compilation check -- larger than "
  "size_t");

int main() {
  return 0;
}
