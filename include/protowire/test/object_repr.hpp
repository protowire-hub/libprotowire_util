/**
 * @file object_repr.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief Customizable string representation of aribtrary objects
 * @version 0.1
 * @date 2025-12-30
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
#include <iomanip>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <type_traits>

#include <boost/nowide/stackstring.hpp>

#include <protowire/test/type_repr.hpp>
#include <protowire/util/lstring.hpp>

namespace protowire {
namespace test {
namespace object_repr {

using protowire::util::lstring::LString;
using protowire::test::type_repr::type_repr;
using protowire::test::type_repr::with_repr_stream;

template <typename T, LString Name>
struct const_name_repr {
  // note usage for nullopt_t, nullptr_t

  static std::string apply(T const&) { return Name.template pack<std::string>(); }

  template <typename CharT, typename Traits>
  static void apply(T const&, std::basic_ostream<CharT, Traits>& b) {
    using str_type = std::basic_string<CharT, Traits>;
    b << Name.template pack<str_type>();
  }
};

template <typename T>
struct object_repr {
  static std::string apply(T const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  template <typename CharT, typename Traits>
  static void apply(T const& ob, std::basic_ostream<CharT, Traits>& b) {
    b << type_repr<T>::apply();
    b << "{{???}}}";
  }
};

template <typename T>
  requires (std::is_const_v<T>)
struct object_repr<T> {
  using value_type = T;

  static std::string apply(value_type& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  template <typename CharT, typename Traits>
  static void apply(value_type& ob, std::basic_ostream<CharT, Traits>& b) {
    b << object_repr<std::remove_cv_t<T>>::apply(ob);
    b << " /** " << type_repr<T>::apply() << " **/";
  }
};

template <typename T>
  requires (std::is_convertible_v<
            std::void_t<decltype(std::to_string(std::declval<T const&>()))>, void
  >)
struct object_repr<T> {
  static std::string apply(T const& ob) { return std::to_string(ob); }

  template <typename CharT, typename Traits>
  static void apply(T const& ob, std::basic_ostream<CharT, Traits>& b) {
    b << type_repr<T>::apply();
    b << "{{";
    b << std::to_string(ob);
    b << "}}";
  }
};

template <typename T>
std::string repr_string(T const& ob) {
  return object_repr<T>::apply(ob);
}

template <typename T, typename CharT, typename Traits>
void repr_write(T const& ob, std::basic_ostream<CharT, Traits>& stream) {
  object_repr<T>::apply(ob, stream);
}

template <typename T, typename C>
struct string_prefix : type_repr<C> {};

template <typename C>
struct string_prefix<char, C> {
  static std::string const apply() { return ""; }
};

template <typename C>
struct string_prefix<wchar_t, C> {
  static std::string const apply() { return "L"; }
};

template <typename C>
struct string_prefix<char8_t, C> {
  static std::string const apply() { return "u8"; }
};

template <typename C>
struct string_prefix<char16_t, C> {
  static std::string const apply() { return "u"; }
};

template <typename C>
struct string_prefix<char32_t, C> {
  static std::string const apply() { return "U"; }
};

template <typename CharT, typename C>
struct text_repr {
  using value_type = C;

  static std::string apply(value_type const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static CharT const* data(value_type const& obj)
    requires (std::is_convertible_v<decltype(std::declval<value_type const&>().data()),
                                    CharT const*>) {
    return obj.data();
  }

  static void apply(value_type const& ob, std::ostream& b) {

    b << type_repr<C>::apply();
    b << "{{";
    if constexpr (std::is_same_v<CharT, char>) {
      b << std::quoted(ob);
    } else {
      b << string_prefix<CharT, value_type>::apply();
      // this assumes UTF-8 encoding for result strings
      b << std::quoted(boost::nowide::basic_stackstring<char, CharT>(data(ob)).get());
    }
    b << "}}";
  }
};

template <typename CharT, typename Traits, typename Alloc>
struct object_repr<std::basic_string<CharT, Traits, Alloc>>
    : text_repr<CharT, std::basic_string<CharT, Traits, Alloc>> {};

template <typename CharT, typename Traits>
struct object_repr<std::basic_string_view<CharT, Traits>>
    : text_repr<CharT, std::basic_string_view<CharT, Traits>> {};

template <>
struct object_repr<char> {
  using value_type = char;

  static std::string apply(value_type const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static void apply(value_type const& ob, std::ostream& b) {
    b << '\'';
    b << ob;
    b << '\'';
  }
};

template <typename CharT>
struct char_repr {
  using value_type = CharT;

  static std::string apply(value_type const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static void apply(value_type const& ob, std::ostream& b) {
    std::basic_string<CharT> s;
    s = ob;
    b << string_prefix<value_type, value_type>::apply();
    b << '\'';
    b << boost::nowide::basic_stackstring<char, CharT>(s.c_str()).get();
    b << '\'';
  }
};

template <>
struct object_repr<wchar_t> : char_repr<wchar_t> {};

template <>
struct object_repr<char8_t> : char_repr<char8_t> {};

template <>
struct object_repr<char16_t> : char_repr<char16_t> {};

template <>
struct object_repr<char32_t> : char_repr<char32_t> {};

template <typename CharT, typename C>
struct char_ptr_repr {
  using value_type = C;

  static std::string apply(value_type const ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static void apply(value_type const ob, std::ostream& b) {
    if constexpr (std::is_same_v<CharT, char>) {
      b << std::quoted(ob);
    } else {
      b << string_prefix<CharT, value_type>::apply();
      b << std::quoted(boost::nowide::basic_stackstring<char, CharT>(ob).get());
    }
  }
};

template <>
struct object_repr<char const*> : char_ptr_repr<char, char const*> {};

template <>
struct object_repr<wchar_t const*> : char_ptr_repr<wchar_t, wchar_t const*> {};

template <>
struct object_repr<char8_t const*> : char_ptr_repr<char8_t, char8_t const*> {};

template <>
struct object_repr<char16_t const*> : char_ptr_repr<char16_t, char16_t const*> {};

template <>
struct object_repr<char32_t const*> : char_ptr_repr<char32_t, char32_t const*> {};

template <typename CharT, typename C>
struct char_nc_ptr_repr {
  using value_type = C;

  static std::string apply(C const ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static void apply(C const ob, std::ostream& b) {
    if constexpr (std::is_same_v<CharT, char>) {
      b << std::quoted(ob);
    } else {
      b << string_prefix<CharT, C>::apply();
      b << std::quoted(boost::nowide::basic_stackstring<char, CharT>(ob).get());
    }
    b << " /** " << type_repr<C>::apply() << " **/";
  }
};

template <>
struct object_repr<char*> : char_nc_ptr_repr<char, char*> {};

template <>
struct object_repr<wchar_t*> : char_nc_ptr_repr<wchar_t, wchar_t*> {};

template <>
struct object_repr<char8_t*> : char_nc_ptr_repr<char8_t, char8_t*> {};

template <>
struct object_repr<char16_t*> : char_nc_ptr_repr<char16_t, char16_t*> {};

template <>
struct object_repr<char32_t*> : char_nc_ptr_repr<char32_t, char32_t*> {};

// --

template <>
struct object_repr<std::nullopt_t> : const_name_repr<std::nullopt_t, "std::nullopt"> {};

template <>
struct object_repr<std::nullptr_t> : const_name_repr<std::nullptr_t, "nullptr"> {};

template <typename T>
struct object_repr<std::optional<T>> {
  using value_type = std::optional<T>;

  static std::string apply(value_type const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  static void apply(value_type const& ob, std::ostream& b) {
    b << type_repr<value_type>::apply();
    if (ob.has_value()) {
      b << "{{";
      object_repr<T>::apply(ob.value(), b);
      b << "}}";
    } else {
      b << "{std::nullopt}";
    }
  }
};

template <typename T>
  requires (std::is_convertible_v<std::remove_pointer_t<typename T::pointer>,
                                  typename T::element_type>)
struct object_repr<T> {
  using value_type = T;

  static std::string apply(value_type const& ob) {
    return with_repr_stream([&ob](std::stringstream& b) { apply(ob, b); });
  }

  void apply(value_type const& ob, std::ostream& b) {
    b << type_repr<value_type>::apply();
    if (ob) {
      b << "{{";
      object_repr<typename T::element_type>::apply(*ob, b);
      b << "}}";
    } else {
      b << "{nullptr}";
    }
  }
};


}  // namespace object_repr
}  // namespace test
}  // namespace protowire
