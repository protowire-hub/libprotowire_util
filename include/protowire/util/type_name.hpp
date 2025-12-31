/**
 * @file type_name.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief utilities for user-visible type names
 * @version 0.2
 * @date 2025-11-28
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
#include <string>
#include <type_traits>
#include <boost/core/demangle.hpp>

namespace protowire {
namespace util {
namespace type_name {

/// @brief utility type for producing a string representing
/// a demangled type name for `T`
///
/// @tparam T type
///
/// Example:
///
/// ```cpp
/// std::cout << type_name<std::optional<int>>::apply();
/// ```
///
/// This implementation uses `boost::core::demagle`
///
template <typename T>
struct type_name {
  static std::string const apply() noexcept {
    return boost::core::demangle(typeid(T).name());
  }
};

/// @brief `type_name` with east `const` syntax
/// @tparam T type
template <typename T>
  requires (std::is_const_v<T>)
struct type_name<T> {
  static std::string const apply() noexcept {
    return type_name<std::remove_cv_t<T>>::apply() + " const";
  }
};

template <typename T>
  requires (std::is_reference_v<T>)
struct type_name<T> {
  static std::string const apply() noexcept {
    return type_name<std::remove_reference_t<T>>::apply() + "&";
  }
};


}  // namespace type_name
}  // namespace util
}  // namespace protowire
