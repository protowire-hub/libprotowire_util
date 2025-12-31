#include <cstdint>
#include <string>
#include <tuple>

#include <boost/mpl/vector.hpp>

#include <protowire/test/typecatch.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("typecatch CHECK_TYPE") {
  CHECK_TYPE(int, int);
  CHECK_TYPE(int*, int*);
  CHECK_TYPE(int&, int&);
  CHECK_TYPE(int const&, int const&);
  CHECK_TYPE(std::string,
             std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
  CHECK_TYPE(std::basic_string<char, std::char_traits<char>, std::allocator<char>>,
             std::string);
  CHECK_TYPE(typename std::string::traits_type, std::char_traits<char>);
  CHECK_TYPE(std::char_traits<char>, typename std::string::traits_type);
}

TEST_CASE("typecatch CHECK_NOT_TYPE") {
  CHECK_NOT_TYPE(int, char);
  CHECK_NOT_TYPE(int*, char*);
  CHECK_NOT_TYPE(int&, char&);
  CHECK_NOT_TYPE(int const&, char const&);
  CHECK_NOT_TYPE(std::wstring,
                 std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
  CHECK_NOT_TYPE(std::basic_string<char, std::char_traits<char>, std::allocator<char>>,
                 std::wstring);
  CHECK_NOT_TYPE(typename std::string::traits_type, std::char_traits<wchar_t>);
  CHECK_NOT_TYPE(std::char_traits<wchar_t>, typename std::string::traits_type);
}

TEST_CASE("typecatch REQUIRE_TYPE") {
  REQUIRE_TYPE(int, int);
  REQUIRE_TYPE(int*, int*);
  REQUIRE_TYPE(int&, int&);
  REQUIRE_TYPE(int const&, int const&);
  REQUIRE_TYPE(std::string,
               std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
  REQUIRE_TYPE(std::basic_string<char, std::char_traits<char>, std::allocator<char>>,
               std::string);
  RECORD_TYPE(typename std::string::traits_type);
  REQUIRE_TYPE(typename std::string::traits_type, std::char_traits<char>);
  REQUIRE_TYPE(std::char_traits<char>, typename std::string::traits_type);
}

TEST_CASE("typecatch REQUIRE_NOT_TYPE") {
  REQUIRE_NOT_TYPE(int, char);
  REQUIRE_NOT_TYPE(int*, char*);
  REQUIRE_NOT_TYPE(int&, char&);
  REQUIRE_NOT_TYPE(int const&, char const&);
  REQUIRE_NOT_TYPE(std::wstring,
                   std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
  REQUIRE_NOT_TYPE(std::basic_string<char, std::char_traits<char>, std::allocator<char>>,
                   std::wstring);
  CAPTURE_TYPE(typename std::wstring::traits_type);
  REQUIRE_NOT_TYPE(typename std::string::traits_type, std::char_traits<wchar_t>);
  REQUIRE_NOT_TYPE(std::char_traits<wchar_t>, typename std::string::traits_type);
}

TEST_CASE("typecatch CHECK_TYPE_CONVERTIBLE") {
  CHECK_TYPE_CONVERTIBLE(std::size_t, int);
}

TEST_CASE("typecatch CHECK_TYPE_NOT_CONVERTIBLE") {
  CHECK_TYPE_NOT_CONVERTIBLE(char*, std::size_t);
}

TEST_CASE("typecatch REQUIRE_TYPE_CONVERTIBLE") {
  REQUIRE_TYPE_CONVERTIBLE(std::size_t, int);
}

TEST_CASE("typecatch REQUIRE_TYPE_NOT_CONVERTIBLE") {
  REQUIRE_TYPE_NOT_CONVERTIBLE(char*, std::size_t);
}

TEST_CASE("typecatch CAPTURE_REPR, RECORD_REPR") {
  CAPTURE_REPR("ABC");
  RECORD_REPR("ABC");
}
