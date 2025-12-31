#pragma once

#include <string>
#include <boost/preprocessor/cat.hpp>
#include <catch2/catch_test_macros.hpp>
#include <protowire/test/object_repr.hpp>

#ifndef CHECK_STR_FIND
#define CHECK_STR_FIND(str, ...)                                                         \
  {                                                                                      \
    UNSCOPED_INFO("CHECK_STR_FIND(" << #str << ", " << #__VA_ARGS__ << ")");             \
    ::std::string const BOOST_PP_CAT(__test_str_, __LINE__) = str;                       \
    UNSCOPED_INFO(#str << " => "                                                         \
                       << ::protowire::test::object_repr::repr_string(                   \
                                BOOST_PP_CAT(__test_str_, __LINE__)));                   \
    CHECK(!!(BOOST_PP_CAT(__test_str_, __LINE__).find(__VA_ARGS__)                       \
             != BOOST_PP_CAT(__test_str_, __LINE__).npos));                              \
  }
#endif
