/**
 * @file lstring.hpp
 * @author Sean Champ <spchamp@users.noreply.github.com>
 * @brief Literal string types
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

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <stdexcept>
#include <type_traits>

#include <boost/mpl/size_t.hpp>

namespace protowire {
namespace util {
namespace lstring {

namespace mpl = boost::mpl;

/// @brief Literal string type
///
/// @tparam N Size of the literal string, generally including a trailing
/// null byte.
///
/// @tparam CharT Character type for the literal string, generally may
/// be inferred
///  though the constructor.
template <std::size_t N, typename CharT>
struct LBasicString {
  using char_type = CharT;
  using data_type = CharT[N];
  using count_type = mpl::size_t<N>;

  CharT data[N];

  constexpr LBasicString(CharT const (&chars)[N]) noexcept {
    std::copy_n(chars, N, data);
  }

  constexpr data_type const& get_value() const { return data; }

  template <typename T>
    requires (std::is_convertible_v<data_type, T>)
  T constexpr pack() const {
    return T{data};
  }

  template <std::size_t ExtN>
    requires (ExtN == N)
  bool operator==(LBasicString<ExtN, CharT> const& ext) const {
    return data == ext.data;
  }

  template <std::size_t ExtN>
    requires (ExtN != N)
  bool operator==(LBasicString<ExtN, CharT> const&) const {
    return false;
  }

  template <typename Traits, typename Alloc>
  bool operator==(std::basic_string<CharT, Traits, Alloc> const& s) const {
    if ((s.size() + 1) == N) {
      std::size_t n = 0;
      for (CharT const& c : s) {
        if (data[n] != c) { return false; }
        n++;
      }
      return true;
    }
    return false;
  }

  template <typename Traits>
  bool operator==(std::basic_string_view<CharT, Traits> const& s) const {
    if ((s.size() + 1) == N) {
      std::size_t n = 0;
      for (CharT const& c : s) {
        if (data[n] != c) { return false; }
        n++;
      }
      return true;
    }
    return false;
  }
};

template <std::size_t N>
struct LString : LBasicString<N, char> {
  using LBasicString<N, char>::data;

  constexpr LString(char const (&chars)[N]) noexcept
      : LBasicString<N, char>(chars) {};

  template <typename Traits, typename Alloc>
  bool operator==(std::basic_string<char, Traits, Alloc> const& s) const {
    return ((s.size() + 1) == N) && (std::strncmp(s.c_str(), data, N) == 0);
  }

  template <typename Traits>
  bool operator==(std::basic_string_view<char, Traits> const& s) const {
    return ((s.size() + 1) == N) && (std::strncmp(s.data(), data, N) == 0);
  }

  template <std::size_t ExtN>
    requires (ExtN == N)
  bool operator==(LString<ExtN> const& ext) const {
    return data == ext.data;
  }

  template <std::size_t ExtN>
    requires (ExtN != N)
  bool operator==(LString<ExtN> const&) const {
    return false;
  }
};

template <std::size_t N>
struct WString : LBasicString<N, wchar_t> {
  using LBasicString<N, wchar_t>::data;

  constexpr WString(wchar_t const (&chars)[N]) noexcept
      : LBasicString<N, wchar_t>(chars) {};

  template <typename Traits, typename Alloc>
  bool operator==(std::basic_string<wchar_t, Traits, Alloc> const& s) const {
    return ((s.size() + 1) == N) && (std::wcsncmp(s.c_str(), data, N) == 0);
  }

  template <typename Traits>
  bool operator==(std::basic_string_view<wchar_t, Traits> const& s) const {
    return ((s.size() + 1) == N) && (std::wcsncmp(s.data(), data, N) == 0);
  }

  template <std::size_t ExtN>
    requires (ExtN == N)
  bool operator==(WString<ExtN> const& ext) const {
    return data == ext.data;
  }

  template <std::size_t ExtN>
    requires (ExtN != N)
  bool operator==(WString<ExtN> const&) const {
    return false;
  }
};

template <std::size_t N>
using U8String = LBasicString<N, char8_t>;


}  // namespace lstring
}  // namespace util
}  // namespace protowire
