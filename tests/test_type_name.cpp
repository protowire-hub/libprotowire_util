// unit tests for type_name

#include <catch2/catch_test_macros.hpp>

#include <protowire/util/type_name.hpp>

using protowire::util::type_name::type_name;

TEST_CASE("type_name") {
  CHECK_NOTHROW(type_name<std::string>::apply());
}
