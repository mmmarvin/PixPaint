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
#ifndef OPTIONAL_H
#define OPTIONAL_H

#include "../os_specific_headers.h"

namespace pixpaint
{
  template<class T>
  class optional
  {
  public:
    using value_type = typename os_specific::optional<T>::value_type;

    optional() noexcept {}
    optional(os_specific::nullopt_t) noexcept : m_value(os_specific::nullopt) {}
    optional(const optional<T>&) = default;
    optional(optional<T>&&) noexcept = default;
    optional& operator=(const optional<T>&) = default;
    optional& operator=(optional<T>&&) noexcept = default;
    template<class U = value_type> optional(U&& value) : m_value(std::forward<U>(value)) {}

    explicit operator bool() const noexcept { return m_value.has_value(); }
    bool has_value() const noexcept { return m_value.has_value(); }

    T& value() { return m_value.value(); }
    const T& value() const { return m_value.value(); }

    void reset() noexcept { m_value.reset(); }

  private:
    os_specific::optional<T> m_value;
  };

  template<class T>
  class optional<T&>
  {
  public:
    using value_type = T;

    optional() noexcept {}
    optional(os_specific::nullopt_t) noexcept : m_value(os_specific::nullopt) {}
    optional(const optional<value_type&>&) = default;
    optional(optional<value_type&>&&) noexcept = default;
    optional& operator=(const optional<value_type&>&) = default;
    optional& operator=(optional<value_type&>&&) noexcept = default;
    template<class U = value_type> optional(U& value) : m_value(&value) {}

    explicit operator bool() const noexcept { return m_value.has_value(); }
    bool has_value() const noexcept { return m_value.has_value(); }

    value_type& value() { return *m_value.value(); }
    const value_type& value() const { return *m_value.value(); }

    void reset() noexcept { m_value.reset(); }

  private:
    os_specific::optional<value_type*> m_value;
  };

  template<class T>
  class optional<const T&>
  {
  public:
    using value_type = const T;

    optional() noexcept {}
    optional(os_specific::nullopt_t) noexcept : m_value(os_specific::nullopt) {}
    optional(const optional<value_type&>&) = default;
    optional(optional<value_type&>&&) noexcept = default;
    optional& operator=(const optional<value_type&>&) = default;
    optional& operator=(optional<value_type&>&&) noexcept = default;
    template<class U = value_type> optional(const U& value) : m_value(&value) {}

    explicit operator bool() const noexcept { return m_value.has_value(); }
    bool has_value() const noexcept { return m_value.has_value(); }

    value_type& value() { return *m_value.value(); }
    const value_type& value() const { return *m_value.value(); }

    void reset() noexcept { m_value.reset(); }

  private:
    os_specific::optional<value_type*> m_value;
  };
}

#endif // OPTIONAL_H
