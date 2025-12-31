// unit tests for type_name

#include <string>
#include <catch2/catch_test_macros.hpp>
#include <protowire/test/check_str.hpp>

#include <protowire/util/type_name.hpp>

using protowire::util::type_name::type_name;

TEST_CASE("type_name") {
  CHECK_STR_FIND(type_name<std::string>::apply(), "string");

  CHECK(type_name<int const&>::apply() == "int const&");

  CHECK(type_name<int&&>::apply() == "int&&");

  CHECK(type_name<int const&&>::apply() == "int const&&");

  CHECK(type_name<char const* const>::apply() == "char const* const");
}
