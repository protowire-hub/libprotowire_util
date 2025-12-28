/**
 * @file tribool_p.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief predicates and operations for tribool values
 * @version 0.1
 * @date 2025-12-27
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
#include <type_traits>

#include <boost/logic/tribool.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>

namespace protowire {
namespace metatypes {

namespace logic = boost::logic;
namespace mpl = boost::mpl;

static constexpr logic::tribool indeterminate_value =
      logic::tribool(logic::indeterminate);
static constexpr logic::tribool true_value = logic::tribool(true);
static constexpr logic::tribool false_value = logic::tribool(false);

template <typename T, T v>
  requires (std::is_convertible_v<T, logic::tribool>)
struct tribool_constant {
  using value_type = T;
  using type = tribool_constant;
  static constexpr T value = v;

  constexpr operator value_type() const { return value; }

  constexpr value_type operator()() const { return value; }

  constexpr tribool_constant() noexcept = default;
};

struct indeterminate_type : tribool_constant<logic::tribool, indeterminate_value> {};

struct true_type : tribool_constant<logic::tribool, true_value> {
  constexpr operator bool() const { return true; }
};

struct false_type : tribool_constant<logic::tribool, false_value> {
  constexpr operator bool() const { return false; }
};

/// @brief unique tag type for a `tribool_` predicate type
struct tribool_tag {};

/// @brief predicate type, generally emulating `mpl::bool_` for tribool objects
template <logic::tribool v>
struct tribool_ {
  constexpr tribool_() noexcept = default;

  static constexpr logic::tribool value = v;
  using tag = tribool_tag;
  using type = tribool_;
  using value_type = logic::tribool;

  constexpr operator logic::tribool() const { return this->value; }
};

template <logic::tribool v>
logic::tribool const tribool_<v>::value;

struct indeterminate_ : tribool_<indeterminate_value> {};

struct true_ : tribool_<true_value> {
  constexpr operator bool() const { return true; }
};

struct false_ : tribool_<false_value> {
  constexpr operator bool() const { return false; }
};

template <logic::tribool C, typename IndT, typename TrueT, typename FalseT>
struct if_indeterminate_c {};

template <typename IndT, typename TrueT, typename FalseT>
struct if_indeterminate_c<indeterminate_value, IndT, TrueT, FalseT> {
  using type = IndT;
};

template <typename IndT, typename TrueT, typename FalseT>
struct if_indeterminate_c<true_value, IndT, TrueT, FalseT> {
  using type = TrueT;
};

template <typename IndT, typename TrueT, typename FalseT>
struct if_indeterminate_c<false_value, IndT, TrueT, FalseT> {
  using type = FalseT;
};

/// @brief  generally emulating boost::mpl::if_ for a tribool-valued type
/// @tparam T predicate type, for member type selection.
///         `T` must provide a static `value` member convertible to a
///         Boost `tribool` object, whether generally indeterminate,
///         or true or false.
/// @tparam IndT member type iff `T::value` is indeterminate
/// @tparam TrueT member type iff `T::value` is true
/// @tparam FalseT member type iff `T::value` is false
template <typename T, typename IndT, typename TrueT, typename FalseT>
  requires (std::is_convertible_v<decltype(T::value), logic::tribool>)
struct if_indeterminate {
  using _intermediate_c = if_indeterminate_c<T::value, IndT, TrueT, FalseT>;
  using type = _intermediate_c::type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(4, if_indeterminate, (T, IndT, TrueT, FalseT))
};


}  // namespace metatypes
}  // namespace protowire
