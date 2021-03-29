/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#ifndef VERSION_SPECIFIC_HEADERS_H
#define VERSION_SPECIFIC_HEADERS_H

#if __cplusplus < 201703L || (defined(_MSC_VER) && (_MSC_VER < 1914))
#define USE_CXX14
#else
#define USE_CXX17
#endif // __cplusplus < 201703L || (defined(_MSC_VER) && (_MSC_VER < 1914))

#if defined(USE_CXX14)
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

namespace version_specific
{
namespace filesystem
{
  using path = boost::filesystem::path;
  using directory_iterator = boost::filesystem::directory_iterator;

  bool exists(const path& filename);
  bool create_directory(const path& p);
  path current_path();
  path temporary_path();
}
  template<class... Args> using variant = boost::variant<Args...>;
  template<class T> using optional = boost::optional<T>;

  using nullopt_t = boost::none_t;
  static const nullopt_t nullopt = boost::none;

  template<class T, class... Args>
  T& variant_get(variant<Args...>& v)
  {
    return boost::get<T>(v);
  }

  template<class T, class... Args>
  const T& variant_get(const variant<Args...>& v)
  {
    return boost::get<T>(v);
  }

  template<std::size_t Index, class... Args>
  decltype(auto) variant_get(variant<Args...>& v)
  {
    return boost::get<Index>(v);
  }

  template<std::size_t Index, class... Args>
  decltype(auto) variant_get(const variant<Args...>& v)
  {
    return boost::get<Index>(v);
  }

  template<class T, class... Args>
  T* variant_get_if(variant<Args...>* v)
  {
    return boost::get<T>(v);
  }

  template<class T, class... Args>
  const T* variant_get_if(const variant<Args...>* v)
  {
    return boost::get<T>(v);
  }

  inline std::string to_string(const filesystem::path& p)
  {
    return p.string();
  }
}
#elif defined(USE_CXX17)
#include <filesystem>
#include <optional>
#include <variant>

namespace version_specific
{
namespace filesystem
{
  using path = std::filesystem::path;
  using directory_iterator = std::filesystem::directory_iterator;

  bool exists(const path& filename);
  bool create_directory(const path& p);
  path current_path();
  path temporary_path();
}
  template<class... Args> using variant = std::variant<Args...>;
  template<class T> using optional = std::optional<T>;

  using nullopt_t = std::nullopt_t;
  static const nullopt_t nullopt = std::nullopt;

  template<class T, class... Args>
  T& variant_get(variant<Args...>& v)
  {
    return std::get<T>(v);
  }

  template<class T, class... Args>
  const T& variant_get(const variant<Args...>& v)
  {
    return std::get<T>(v);
  }

  template<std::size_t Index, class... Args>
  decltype(auto) variant_get(variant<Args...>& v)
  {
    return std::get<Index>(v);
  }

  template<std::size_t Index, class... Args>
  decltype(auto) variant_get(const variant<Args...>& v)
  {
    return std::get<Index>(v);
  }

  template<class T, class... Args>
  T* variant_get_if(variant<Args...>* v)
  {
    return std::get_if<T>(v);
  }

  template<class T, class... Args>
  const T* variant_get_if(const variant<Args...>* v)
  {
    return std::get_if<T>(v);
  }

  inline std::string to_string(const filesystem::path& p)
  {
    return p.string();
  }
}
#endif // __cplusplus < 201703L || (defined(_MSC_VER) && (_MSC_VER < 1914))
#endif // VERSION_SPECIFIC_HEADERS_H
