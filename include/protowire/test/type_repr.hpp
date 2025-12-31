/**
 * @file type_repr.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief Customizable string representation of aribtrary types
 * @version 0.1
 * @date 2025-12-30
 *
 * @copyright Copyright (c) 2025 Sean Champ
 * @par License:
 *
 * This file is part of the libprotowire_util software project.
 * libprotowire_util is free software; you can redistribute it
 * and/or modify it under the terms of the Artistic License 2.0.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the Artistic License 2.0
 * along with libprotowire_util; if not, see
 * <https://github.com/protowire-hub/libprotowire_util/blob/main/COPYING.md>
 *
 */

#pragma once

#include <concepts>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <tuple>
#include <utility>
#include <variant>

#include <protowire/util/type_name.hpp>

namespace protowire {
namespace test {
namespace type_repr {

using protowire::util::type_name::type_name;

/// @brief Default type interface for string representation
/// of arbitrary types
///
/// @tparam T the type for which `apply()` should produce
/// a generally human-readable type name.
///
template <typename T>
struct type_repr {

  /// @brief return a representative string for the type `T`,
  /// using `type_name<T>::apply()`
  static std::string const apply() { return type_name<T>::apply(); };
};

template <typename F>
  requires (std::is_convertible_v<std::invoke_result_t<F, std::stringstream&>, void>)
std::string with_repr_stream(F&& func) {
  std::stringstream b{};
  std::invoke(func, b);
  return b.str();
};

template <typename T>
std::string prefix_repr(std::string_view const& prefix) {
  return with_repr_stream([&prefix](std::ostream& b) {
    b << prefix;
    b << type_repr<T>::apply();
  });
};

template <typename T>
std::string suffix_repr(std::string_view const& suffix) {
  return with_repr_stream([&suffix](std::ostream& b) {
    b << type_repr<T>::apply();
    b << suffix;
  });
};

template <typename T>
std::string infix_repr(std::string_view const& prefix, std::string_view const& suffix) {
  return with_repr_stream([&prefix, &suffix](std::ostream& b) {
    b << prefix;
    b << type_repr<T>::apply();
    b << suffix;
  });
};

template <typename F>
  requires (std::is_convertible_v<std::invoke_result_t<F, std::ostream&>, void>)
std::string template_repr(std::string_view const& template_name, F&& func) {
  return with_repr_stream([&template_name, &func](std::ostream& b) {
    b << template_name << "<";
    std::invoke(func, b);
    b << ">";
  });
};

std::string template_repr(std::string_view const& template_name) {
  return template_repr(template_name, [](std::ostream const&) {});
};

template <typename First, typename... Rest>
std::string template_repr(std::string_view const& template_name) {
  return template_repr(template_name, [](std::ostream& b) {
    b << type_repr<First>::apply();
    ((b << ", " << type_repr<Rest>::apply()), ...);
  });
};

template <typename T>
struct type_repr<T const> {
  static std::string const apply() { return suffix_repr<T>(" const"); }
};

template <typename T>
struct type_repr<T*> {
  static std::string const apply() { return suffix_repr<T>("*"); }
};

template <typename T>
struct type_repr<T const*> {
  static std::string const apply() { return suffix_repr<T>(" const*"); }
};

template <typename T>
struct type_repr<T&> {
  static std::string const apply() { return suffix_repr<T>("&"); }
};

template <typename T>
struct type_repr<T&&> {
  static std::string const apply() { return suffix_repr<T>("&&"); }
};

template <template <typename...> typename Meta, typename... Args>
struct meta_prefix {
  static std::optional<std::string> const apply() {
    std::string const orig_name = type_name<Meta<Args...>>::apply();
    std::size_t const st = orig_name.find('<');
    if (st == orig_name.npos) {
      return std::nullopt;
    } else {
      return orig_name.substr(0, st);
    }
  }
};

template <template <typename...> typename Meta, typename First, typename... Rest>
struct type_repr<Meta<First, Rest...>> {
  static std::string const apply() {
    std::optional<std::string> const meta_name =
          meta_prefix<Meta, First, Rest...>::apply();
    if (meta_name.has_value()) {
      return template_repr<First, Rest...>(meta_name.value());
    }
    return type_name<Meta<First, Rest...>>::apply();
  }
};

template <template <typename...> typename Meta>
struct type_repr<Meta<>> {
  static std::string const apply() { return type_name<Meta<>>::apply(); }
};

template <>
struct type_repr<std::string> {
  static std::string const apply() { return "std::string"; }
};

template <>
struct type_repr<std::wstring> {
  static std::string const apply() { return "std::wstring"; }
};

template <>
struct type_repr<std::u8string> {
  static std::string const apply() { return "std::u8string"; }
};

template <>
struct type_repr<std::u16string> {
  static std::string const apply() { return "std::u16string"; }
};

template <>
struct type_repr<std::u32string> {
  static std::string const apply() { return "std::u32string"; }
};

template <>
struct type_repr<std::string_view> {
  static std::string const apply() { return "std::string_view"; }
};

template <>
struct type_repr<std::wstring_view> {
  static std::string const apply() { return "std::wstring_view"; }
};

template <>
struct type_repr<std::u8string_view> {
  static std::string const apply() { return "std::u8string_view"; }
};

template <>
struct type_repr<std::u16string_view> {
  static std::string const apply() { return "std::u16string_view"; }
};

template <>
struct type_repr<std::u32string_view> {
  static std::string const apply() { return "std::u32string_view"; }
};


}  // namespace type_repr
}  // namespace test
}  // namespace protowire
