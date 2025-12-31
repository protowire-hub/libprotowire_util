#include <string>

#include <protowire/test/object_repr.hpp>

#include <catch2/catch_test_macros.hpp>

#include <boost/preprocessor/cat.hpp>

using protowire::util::type_name::type_name;
using protowire::test::object_repr::object_repr;
using protowire::test::object_repr::repr_string;

#ifndef TEST_REPR_STRING
#define TEST_REPR_STRING(value, expect) CHECK(repr_string((value)) == (expect));
#endif

#ifndef TEST_OBJECT_REPR_C
#define TEST_OBJECT_REPR_C(value, expect)                                                \
  {                                                                                      \
    auto const BOOST_PP_CAT(__value_, __LINE__) = (value);                               \
    using BOOST_PP_CAT(__value_t_, __LINE__) =                                           \
          decltype(BOOST_PP_CAT(__value_, __LINE__));                                    \
    CHECK(object_repr<BOOST_PP_CAT(__value_t_, __LINE__)>::apply(                        \
                BOOST_PP_CAT(__value_, __LINE__))                                        \
          == (expect));                                                                  \
  }
#endif


#ifndef TEST_OBJECT_REPR_NC
#define TEST_OBJECT_REPR_NC(CharT, value, expect)                                        \
  {                                                                                      \
    CharT BOOST_PP_CAT(__arr_, __LINE__)[] = (value);                                    \
    CharT* BOOST_PP_CAT(__value_, __LINE__) = BOOST_PP_CAT(__arr_, __LINE__);            \
    CHECK(object_repr<CharT*>::apply(BOOST_PP_CAT(__value_, __LINE__)) == (expect));     \
  }
#endif

TEST_CASE("object_repr : typographical objects") {

  SECTION("String") {
    TEST_REPR_STRING(std::string{"ABC"}, "std::string{{\"ABC\"}}");
    TEST_REPR_STRING(std::wstring{L"ABC"}, "std::wstring{{L\"ABC\"}}");
    TEST_REPR_STRING(std::u8string{u8"±4Å"}, "std::u8string{{u8\"±4Å\"}}");
    TEST_REPR_STRING(std::u16string{u"±4Å"}, "std::u16string{{u\"±4Å\"}}");
    TEST_REPR_STRING(std::u32string{U"±4Å"}, "std::u32string{{U\"±4Å\"}}");
  }

  SECTION("String View") {
    TEST_REPR_STRING(std::string_view{"ABC"}, "std::string_view{{\"ABC\"}}");
    TEST_REPR_STRING(std::wstring_view{L"ABC"}, "std::wstring_view{{L\"ABC\"}}");
    TEST_REPR_STRING(std::u8string_view{u8"±4Å"}, "std::u8string_view{{u8\"±4Å\"}}");
    TEST_REPR_STRING(std::u16string_view{u"±4Å"}, "std::u16string_view{{u\"±4Å\"}}");
    TEST_REPR_STRING(std::u32string_view{U"±4Å"}, "std::u32string_view{{U\"±4Å\"}}");
  }

  SECTION("Char") {
    TEST_REPR_STRING('x', "\'x\'");
    TEST_REPR_STRING(L'x', "L\'x\'");
    TEST_REPR_STRING(u8'x', "u8\'x\'");
    TEST_REPR_STRING(u'±', "u\'±\'");
    TEST_REPR_STRING(U'±', "U\'±\'");
  }

  auto p = "abc";

  SECTION("Platform test: char const pointer") {
    REQUIRE(type_name<decltype(p)>::apply() == "char const*");
  }

  CHECKED_IF(type_name<decltype(p)>::apply() == "char const*") {
    SECTION("Char const pointer") {
      TEST_REPR_STRING("ABC", "\"ABC\"");
      TEST_REPR_STRING(L"ABC", "L\"ABC\"");
      TEST_REPR_STRING(u8"ABC", "u8\"ABC\"");
      TEST_REPR_STRING(u"ABC", "u\"ABC\"");
      TEST_REPR_STRING(U"ABC", "U\"ABC\"");
    }
  }

  SECTION("Char const pointer (const)") {
    TEST_OBJECT_REPR_C("ABC", "\"ABC\" /** char const* const **/");
    TEST_OBJECT_REPR_C(L"ABC", "L\"ABC\" /** wchar_t const* const **/");
    TEST_OBJECT_REPR_C(u8"ABC", "u8\"ABC\" /** char8_t const* const **/");
    TEST_OBJECT_REPR_C(u"ABC", "u\"ABC\" /** char16_t const* const **/");
    TEST_OBJECT_REPR_C(U"ABC", "U\"ABC\" /** char32_t const* const **/");
  }

  SECTION("Char pointer") {
    TEST_OBJECT_REPR_NC(char, "ABC", "\"ABC\" /** char* **/");
    TEST_OBJECT_REPR_NC(wchar_t, L"ABC", "L\"ABC\" /** wchar_t* **/");
    TEST_OBJECT_REPR_NC(char8_t, u8"ABC", "u8\"ABC\" /** char8_t* **/");
    TEST_OBJECT_REPR_NC(char16_t, u"ABC", "u\"ABC\" /** char16_t* **/");
    TEST_OBJECT_REPR_NC(char32_t, U"ABC", "U\"ABC\" /** char32_t* **/");
  }
}
