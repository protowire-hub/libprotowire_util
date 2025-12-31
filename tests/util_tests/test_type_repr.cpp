
#include <protowire/util/type_repr.hpp>

#include <catch2/catch_test_macros.hpp>

template <typename Item>
struct custom_unary_t {};

template <typename... Args>
struct custom_nary_t {};

using protowire::util::type_repr::type_repr;

TEST_CASE("type_repr for standard types") {
  CHECK(type_repr<int>::apply() == "int");
  CHECK(type_repr<std::optional<int>>::apply() == "std::optional<int>");
  CHECK(type_repr<std::variant<int, char>>::apply() == "std::variant<int, char>");
  CHECK(type_repr<int&>::apply() == "int&");
  CHECK(type_repr<int&&>::apply() == "int&&");
  CHECK(type_repr<std::string>::apply() == "std::string");
  CHECK(type_repr<std::wstring>::apply() == "std::wstring");

  CHECK(type_repr<char const* const>::apply() == "char const* const");
  CHECK(type_repr<std::variant<int const*, char const*>>::apply()
        == "std::variant<int const*, char const*>");
  CHECK(type_repr<int const&>::apply() == "int const&");
  CHECK(type_repr<int const&&>::apply() == "int const&&");

  CHECK(type_repr<std::optional<std::pair<int, std::string>>>::apply()
        == "std::optional<std::pair<int, std::string>>");
  CHECK(type_repr<std::variant<int, std::string>>::apply()
        == "std::variant<int, std::string>");
}

TEST_CASE("type_repr for arbitrary template parameter pack") {
  CHECK(type_repr<custom_unary_t<int>>::apply() == "custom_unary_t<int>");
  CHECK(type_repr<custom_nary_t<>>::apply() == "custom_nary_t<>");
  CHECK(type_repr<custom_nary_t<int>>::apply() == "custom_nary_t<int>");
  CHECK(type_repr<custom_nary_t<int const*, char const*>>::apply()
        == "custom_nary_t<int const*, char const*>");
}
