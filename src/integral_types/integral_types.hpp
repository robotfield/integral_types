#ifndef INTEGRAL_TYPES_HPP
#define INTEGRAL_TYPES_HPP

#if __cplusplus < 201103L
#error "integral_types.hpp: this header relies on C++11 features"
#endif

#include <climits>
#include <type_traits>

namespace integral_types {

// bit_sizeof
template <typename T>
struct bit_sizeof {
  static constexpr std::size_t value = CHAR_BIT * sizeof(T);
};

#if __cplusplus >= 201703L
template <typename T>
inline constexpr std::size_t bit_sizeof_v = bit_sizeof<T>::value;
#elif __cplusplus >= 201402L
template <typename T>
constexpr std::size_t bit_sizeof_v = bit_sizeof<T>::value;
#endif

// find_exact_bit_sizeof
template <std::size_t bits, typename... Ts>
struct find_exact_bit_sizeof {
  static_assert(
    bits && false,
    "integral_types.hpp: could not find a type with that exact bit size");
};

template <typename T, typename... Ts>
struct find_exact_bit_sizeof<
  static_cast<std::size_t>(bit_sizeof<T>::value),
  T,
  Ts...> {
  using type = T;
};

template <std::size_t bits, typename T, typename... Ts>
struct find_exact_bit_sizeof<bits, T, Ts...>
  : find_exact_bit_sizeof<bits, Ts...> {};

template <std::size_t bits, typename... Ts>
using find_exact_bit_sizeof_t = typename find_exact_bit_sizeof<bits, Ts...>::
  type;

// find_first_geq_bit_size C++20
#if __cplusplus >= 202002L
template <std::size_t bits, typename... Ts>
struct find_first_geq_bit_size {
  static_assert(
    bits && false,
    "integral_types.hpp: could not find a type greater than or equal to that "
    "exact bit size");
};

template <std::size_t bits, typename T, typename... Ts>
  requires(bits <= bit_sizeof_v<T>)
struct find_first_geq_bit_size<bits, T, Ts...> {
  using type = T;
};

template <std::size_t bits, typename T, typename... Ts>
  requires(bits > bit_sizeof_v<T>)
struct find_first_geq_bit_size<bits, T, Ts...>
  : find_first_geq_bit_size<bits, Ts...> {};
#else
// find_first_geq_bit_size pre-C++20
namespace integral_types_helper {
template <typename Enable, std::size_t bits, typename... Ts>
struct find_first_geq_bit_sizeof_helper {
  static_assert(
    bits && false,
    "integral_types.hpp: could not find a type greater than or equal to that "
    "exact bit size");
};

template <std::size_t bits, typename T, typename... Ts>
struct find_first_geq_bit_sizeof_helper<
  typename std::enable_if<(bits <= bit_sizeof<T>::value)>::type,
  bits,
  T,
  Ts...> {
  using type = T;
};

template <std::size_t bits, typename T, typename... Ts>
struct find_first_geq_bit_sizeof_helper<
  typename std::enable_if<(bits > bit_sizeof<T>::value)>::type,
  bits,
  T,
  Ts...> : find_first_geq_bit_sizeof_helper<void, bits, Ts...> {};
} // namespace integral_types_helper

template <std::size_t bits, typename... Ts>
struct find_first_geq_bit_size
  : integral_types_helper::find_first_geq_bit_sizeof_helper<void, bits, Ts...> {
};
#endif

template <std::size_t bits, typename... Ts>
using find_first_geq_bit_sizeof_t =
  typename find_first_geq_bit_size<bits, Ts...>::type;

// find_first_leq_bit_size
#if __cplusplus >= 202002L
template <std::size_t bits, typename... Ts>
struct find_first_leq_bit_size {
  static_assert(
    bits && false,
    "integral_types.hpp: could not find a type less than or equal to that "
    "exact bit size");
};

template <std::size_t bits, typename T, typename... Ts>
  requires(bits >= bit_sizeof_v<T>)
struct find_first_leq_bit_size<bits, T, Ts...> {
  using type = T;
};

template <std::size_t bits, typename T, typename... Ts>
  requires(bits < bit_sizeof_v<T>)
struct find_first_leq_bit_size<bits, T, Ts...>
  : find_first_leq_bit_size<bits, Ts...> {};

#else
namespace integral_types_helper {
template <typename Enable, std::size_t bits, typename... Ts>
struct find_first_leq_bit_sizeof_helper {
  static_assert(
    bits && false,
    "integral_types.hpp: could not find a type less than or equal to that "
    "exact bit size");
};

template <std::size_t bits, typename T, typename... Ts>
struct find_first_leq_bit_sizeof_helper<
  typename std::enable_if<(bits >= bit_sizeof<T>::value)>::type,
  bits,
  T,
  Ts...> {
  using type = T;
};

template <std::size_t bits, typename T, typename... Ts>
struct find_first_leq_bit_sizeof_helper<
  typename std::enable_if<(bits < bit_sizeof<T>::value)>::type,
  bits,
  T,
  Ts...> : find_first_leq_bit_sizeof_helper<void, bits, Ts...> {};
} // namespace integral_types_helper

template <std::size_t bits, typename... Ts>
struct find_first_leq_bit_size
  : integral_types_helper::find_first_leq_bit_sizeof_helper<void, bits, Ts...> {
};
#endif

template <std::size_t bits, typename... Ts>
using find_first_leq_bit_sizeof_t =
  typename find_first_leq_bit_size<bits, Ts...>::type;

// Integral Types

// Integral types matching the supplied number of bits exactly, compilation
// error otherwise
template <std::size_t bits>
using int_t = find_exact_bit_sizeof_t<
  bits,
  signed char,
  signed short,
  signed int,
  signed long,
  signed long long>;
template <std::size_t bits>
using uint_t = find_exact_bit_sizeof_t<
  bits,
  unsigned char,
  unsigned short,
  unsigned int,
  unsigned long,
  unsigned long long>;

// Integral types with at least the supplied number of bits, compilation error
// otherwise
template <std::size_t bits>
using int_least_t = find_first_geq_bit_sizeof_t<
  bits,
  signed char,
  signed short,
  signed int,
  signed long,
  signed long long>;
template <std::size_t bits>
using uint_least_t = find_first_geq_bit_sizeof_t<
  bits,
  unsigned char,
  unsigned short,
  unsigned int,
  unsigned long,
  unsigned long long>;

// Integral types of a convenient size for the processor to manipulate
using int_fit_t = find_first_leq_bit_size<
  bit_sizeof<std::size_t>::value,
  signed long long,
  signed long,
  signed int,
  signed short,
  signed char>;
using uint_fit_t = find_first_leq_bit_size<
  bit_sizeof<std::size_t>::value,
  unsigned long long,
  unsigned long,
  unsigned int,
  unsigned short,
  unsigned char>;

} // namespace integral_types
#endif
