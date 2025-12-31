/**
 * @file typecatch.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief type-oriented extensions for Catch2
 * @version 0.1
 * @date 2025-12-31
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

#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include <protowire/test/type_repr.hpp>
#include <protowire/test/object_repr.hpp>
#include <protowire/test/equal_op.hpp>

namespace protowire {
namespace test {
namespace typecatch {

using protowire::test::type_repr::type_repr;
using protowire::test::object_repr::repr_string;
using protowire::test::object_repr::object_repr;

template <typename T, typename Expected>
struct expect_info {
  static void notify() {
    UNSCOPED_INFO("Type:        " << type_repr<T>::apply());
    UNSCOPED_INFO("Expect Type: " << type_repr<Expected>::apply());
  };
};

template <typename T, typename U>
struct convert_info {
  static void notify() {
    UNSCOPED_INFO("T: " << type_repr<T>::apply());
    UNSCOPED_INFO("U: " << type_repr<U>::apply());
  };
};

template <typename T, typename Expected>
struct expect_not_info {
  static void notify() {
    UNSCOPED_INFO("Type:            " << type_repr<T>::apply());
    UNSCOPED_INFO("Expect Not Type: " << type_repr<Expected>::apply());
  };
};


}  // namespace typecatch
}  // namespace test
}  // namespace protowire

#ifndef CHECK_TYPE
#define CHECK_TYPE(...)                                                                  \
  UNSCOPED_INFO("CHECK_TYPE(" << #__VA_ARGS__ << ")");                                   \
  ::protowire::test::typecatch::expect_info<__VA_ARGS__>::notify();                      \
  CHECK(::std::is_same_v<__VA_ARGS__>)
#endif

#ifndef REQUIRE_TYPE
#define REQUIRE_TYPE(...)                                                                \
  UNSCOPED_INFO("REQUIRE_TYPE(" << #__VA_ARGS__ << ")");                                 \
  ::protowire::test::typecatch::expect_info<__VA_ARGS__>::notify();                      \
  REQUIRE(::std::is_same_v<__VA_ARGS__>)
#endif


#ifndef CHECK_NOT_TYPE
#define CHECK_NOT_TYPE(...)                                                              \
  UNSCOPED_INFO("CHECK_NOT_TYPE(" << #__VA_ARGS__ << ")");                               \
  ::protowire::test::typecatch::expect_not_info<__VA_ARGS__>::notify();                  \
  CHECK(!::std::is_same_v<__VA_ARGS__>)
#endif


#ifndef REQUIRE_NOT_TYPE
#define REQUIRE_NOT_TYPE(...)                                                            \
  UNSCOPED_INFO("REQUIRE_NOT_TYPE(" << #__VA_ARGS__ << ")");                             \
  ::protowire::test::typecatch::expect_not_info<__VA_ARGS__>::notify();                  \
  REQUIRE(!::std::is_same_v<__VA_ARGS__>)
#endif


#ifndef CHECK_TYPE_CONVERTIBLE
#define CHECK_TYPE_CONVERTIBLE(...)                                                      \
  UNSCOPED_INFO("CHECK_TYPE_CONVERTIBLE(" << #__VA_ARGS__ << ")");                       \
  ::protowire::test::typecatch::convert_info<__VA_ARGS__>::notify();                     \
  CHECK(::std::is_convertible_v<__VA_ARGS__>)
#endif

#ifndef REQUIRE_TYPE_CONVERTIBLE
#define REQUIRE_TYPE_CONVERTIBLE(...)                                                    \
  UNSCOPED_INFO("REQUIRE_TYPE_CONVERTIBLE(" << #__VA_ARGS__ << ")");                     \
  ::protowire::test::typecatch::convert_info<__VA_ARGS__>::notify();                     \
  REQUIRE(::std::is_convertible_v<__VA_ARGS__>)
#endif

#ifndef CHECK_TYPE_NOT_CONVERTIBLE
#define CHECK_TYPE_NOT_CONVERTIBLE(...)                                                  \
  UNSCOPED_INFO("CHECK_TYPE_NOT_CONVERTIBLE(" << #__VA_ARGS__ << ")");                   \
  ::protowire::test::typecatch::convert_info<__VA_ARGS__>::notify();                     \
  CHECK(!::std::is_convertible_v<__VA_ARGS__>)
#endif

#ifndef REQUIRE_TYPE_NOT_CONVERTIBLE
#define REQUIRE_TYPE_NOT_CONVERTIBLE(...)                                                \
  UNSCOPED_INFO("REQUIRE_TYPE_NOT_CONVERTIBLE(" << #__VA_ARGS__ << ")");                 \
  ::protowire::test::typecatch::convert_info<__VA_ARGS__>::notify();                     \
  REQUIRE(!::std::is_convertible_v<__VA_ARGS__>)
#endif

#ifndef RECORD_TYPE
#define RECORD_TYPE(...)                                                                 \
  UNSCOPED_INFO(                                                                         \
        #__VA_ARGS__ << " := "                                                           \
                     << ::protowire::test::typecatch::type_repr<__VA_ARGS__>::apply());
#endif

#ifndef CAPTURE_TYPE
#define CAPTURE_TYPE(...)                                                                \
  INFO(#__VA_ARGS__ << " := "                                                            \
                    << ::protowire::test::typecatch::type_repr<__VA_ARGS__>::apply())
#endif

#ifndef RECORD_REPR
#define RECORD_REPR(...)                                                                 \
  UNSCOPED_INFO(                                                                         \
        #__VA_ARGS__ << " := "                                                           \
                     << ::protowire::test::object_repr::repr_string(__VA_ARGS__))
#endif

#ifndef CAPTURE_REPR
#define CAPTURE_REPR(...)                                                                \
  INFO(#__VA_ARGS__ << " := " << ::protowire::test::object_repr::repr_string(__VA_ARGS__))
#endif
