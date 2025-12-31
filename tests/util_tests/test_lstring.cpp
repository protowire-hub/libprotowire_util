

#include <string>
#include <string_view>

#include <protowire/util/lstring.hpp>

#include <catch2/catch_test_macros.hpp>

using protowire::util::lstring::LBasicString;
using protowire::util::lstring::LString;
using protowire::util::lstring::WString;
using protowire::util::lstring::U8String;

template <typename T, T Name>
struct test_adapter {
  using ls_type = T;

  template <typename Traits, typename Alloc>
  static bool cmp(std::basic_string<typename T::char_type, Traits, Alloc> const& s) {
    return Name == s;
  }

  template <typename Traits>
  static bool cmp(std::basic_string_view<typename T::char_type, Traits> const& s) {
    return Name == s;
  }

  template <typename U, U UName>
  static bool cmp(test_adapter<U, UName> const&) {
    return Name == UName;
  }

  template <typename U>
  static constexpr U const pack() {
    return Name.template pack<U>();
  }
};

template <LBasicString Name>
using b_param_test = test_adapter<decltype(Name), Name>;

template <LString Name>
using s_param_test = test_adapter<decltype(Name), Name>;

template <WString Name>
using w_param_test = test_adapter<decltype(Name), Name>;

template <U8String Name>
using u8_param_test = test_adapter<decltype(Name), Name>;

TEST_CASE("LBasicString") {

  SECTION("char") {
    using data_t = b_param_test<"abc">;
    using other_t = b_param_test<"xyz">;
    CHECK(data_t::ls_type::count_type::value == 4);

    CHECK(data_t::pack<std::string>() == "abc");
    CHECK(data_t::pack<std::string>().size() == 3);

    CHECK(data_t::pack<std::string_view>() == "abc");
    CHECK(data_t::pack<std::string_view>().size() == 3);

    CHECK(data_t::cmp(data_t::pack<std::string>()));
    CHECK(data_t::cmp(data_t::pack<std::string_view>()));
    CHECK(!data_t::cmp(std::string{"xyz"}));
    CHECK(!data_t::cmp(std::string_view{"xyz"}));

    CHECK(data_t::cmp(data_t{}));
    CHECK(!data_t::cmp(other_t{}));
  }


  SECTION("wchar_t") {
    using data_t = b_param_test<L"abc">;
    using other_t = b_param_test<L"xyz">;
    CHECK(data_t::ls_type::count_type::value == 4);

    CHECK(data_t::pack<std::wstring>() == L"abc");
    CHECK(data_t::pack<std::wstring>().size() == 3);
    CHECK(data_t::pack<std::wstring_view>() == L"abc");
    CHECK(data_t::pack<std::wstring_view>().size() == 3);

    CHECK(data_t::cmp(data_t::pack<std::wstring>()));
    CHECK(data_t::cmp(data_t::pack<std::wstring_view>()));
    CHECK(!data_t::cmp(std::wstring{L"xyz"}));
    CHECK(!data_t::cmp(std::wstring_view{L"xyz"}));

    CHECK(data_t::cmp(data_t{}));
    CHECK(!data_t::cmp(other_t{}));
  }


  SECTION("char8_t") {
    using data_t = b_param_test<u8"地質">;
    using other_t = b_param_test<u8"地地地">;

    CHECK(data_t::ls_type::count_type::value >= 2);
    CHECK(data_t::pack<std::u8string>() == u8"地質");
    CHECK(data_t::pack<std::u8string_view>() == u8"地質");

    CHECK(data_t::cmp(data_t::pack<std::u8string>()));
    CHECK(data_t::cmp(data_t::pack<std::u8string_view>()));
    CHECK(!data_t::cmp(std::u8string{u8"xyz"}));
    CHECK(!data_t::cmp(std::u8string_view{u8"xyz"}));

    CHECK(data_t::cmp(data_t{}));
    CHECK(!data_t::cmp(other_t{}));
  }

  SECTION("char16_t") {
    using data_t = b_param_test<u"地質">;
    using other_t = b_param_test<u"地地地">;

    CHECK(data_t::ls_type::count_type::value >= 2);
    CHECK(data_t::pack<std::u16string>() == u"地質");
    CHECK(data_t::pack<std::u16string_view>() == u"地質");

    CHECK(data_t::cmp(data_t::pack<std::u16string>()));
    CHECK(data_t::cmp(data_t::pack<std::u16string_view>()));
    CHECK(!data_t::cmp(std::u16string{u"xyz"}));
    CHECK(!data_t::cmp(std::u16string_view{u"xyz"}));

    CHECK(data_t::cmp(data_t{}));
    CHECK(!data_t::cmp(other_t{}));
  }
}

TEST_CASE("SString") {
  using data_t = s_param_test<"ABC">;
  using other_t = s_param_test<"XYZ">;

  CHECK(data_t::ls_type::count_type::value == 4);
  CHECK(data_t::pack<std::string>() == "ABC");
  CHECK(data_t::pack<std::string_view>() == "ABC");

  CHECK(data_t::cmp(data_t::pack<std::string>()));
  CHECK(data_t::cmp(data_t::pack<std::string_view>()));
  CHECK(!data_t::cmp(std::string{"xyz"}));
  CHECK(!data_t::cmp(std::string_view{"xyz"}));

  CHECK(data_t::cmp(data_t{}));
  CHECK(!data_t::cmp(other_t{}));
}

TEST_CASE("WString") {
  using data_t = w_param_test<L"ABC">;
  using other_t = w_param_test<L"XYZ">;

  CHECK(data_t::ls_type::count_type::value == 4);
  CHECK(data_t::pack<std::wstring>() == L"ABC");
  CHECK(data_t::pack<std::wstring_view>() == L"ABC");

  CHECK(data_t::pack<std::wstring>() == L"ABC");
  CHECK(data_t::pack<std::wstring>().size() == 3);
  CHECK(data_t::pack<std::wstring_view>() == L"ABC");
  CHECK(data_t::pack<std::wstring_view>().size() == 3);

  CHECK(data_t::cmp(data_t::pack<std::wstring>()));
  CHECK(data_t::cmp(data_t::pack<std::wstring_view>()));
  CHECK(!data_t::cmp(std::wstring{L"xyz"}));
  CHECK(!data_t::cmp(std::wstring_view{L"xyz"}));

  CHECK(data_t::cmp(data_t{}));
  CHECK(!data_t::cmp(other_t{}));
}

TEST_CASE("U8String") {
  using data_t = u8_param_test<u8"ABC">;
  using other_t = u8_param_test<u8"XYZ">;

  CHECK(data_t::ls_type::count_type::value == 4);
  CHECK(data_t::pack<std::u8string>() == u8"ABC");
  CHECK(data_t::pack<std::u8string_view>() == u8"ABC");

  CHECK(data_t::cmp(data_t::pack<std::u8string>()));
  CHECK(data_t::cmp(data_t::pack<std::u8string_view>()));
  CHECK(!data_t::cmp(std::u8string{u8"xyz"}));
  CHECK(!data_t::cmp(std::u8string_view{u8"xyz"}));

  CHECK(data_t::cmp(data_t{}));
  CHECK(!data_t::cmp(other_t{}));
}
