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
#include "registrarbase.h"

#include "../manager/consolemanager.h"

namespace pixpaint
{
  template<class T>
  template<class... Args>
  bool RegistrarBase<T>::registerBuiltIn(std::string id, Args&&... args)
  {
    if(id.size() > registerbase_detail::ID_SIZE_LIMIT) {
      getConsoleManager().writeMessage("Cannot register id \"" +
                                       id +
                                       "\" because it exceed maximum limit for size (>" +
                                       std::to_string(registerbase_detail::ID_SIZE_LIMIT) +
                                       " characters long)");
      return false;
    }

    if(m_ids.find(id) != m_ids.end()) {
      getConsoleManager().writeMessage("Cannot register id \"" +
                                       id +
                                       "\" because it is alread taken!");
      return false;
    }

    m_items.emplace_back(std::forward<Args>(args)...);
    m_ids.emplace(std::move(id));

    return true;
  }

  template<class T>
  template<class... Args>
  bool RegistrarBase<T>::registerCustom(std::string id, Args&&... args)
  {
    if(id.size() > registerbase_detail::ID_SIZE_LIMIT) {
      getConsoleManager().writeMessage("Cannot register id \"" +
                                       id +
                                       "\" because it exceed maximum limit for size (>" +
                                       std::to_string(registerbase_detail::ID_SIZE_LIMIT) +
                                       " characters long)");
      return false;
    }

    if(m_ids.find(id) != m_ids.end()) {
      getConsoleManager().writeMessage("Cannot register id \"" +
                                       id +
                                       "\" because it is alread taken!");
      return false;
    }

    m_items.emplace_back(std::forward<Args>(args)...);
    m_ids.emplace(std::move(id));

    return true;
  }

  template<class T>
  typename RegistrarBase<T>::container_type::iterator RegistrarBase<T>::begin()
  {
    return m_items.begin();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::const_iterator RegistrarBase<T>::begin() const
  {
    return m_items.begin();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::const_iterator RegistrarBase<T>::cbegin() const
  {
    return m_items.cbegin();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::reverse_iterator RegistrarBase<T>::rbegin()
  {
    return m_items.rbegin();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::iterator RegistrarBase<T>::end()
  {
    return m_items.end();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::const_iterator RegistrarBase<T>::end() const
  {
    return m_items.end();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::const_iterator RegistrarBase<T>::cend() const
  {
    return m_items.cend();
  }

  template<class T>
  typename RegistrarBase<T>::container_type::reverse_iterator RegistrarBase<T>::rend()
  {
    return m_items.rend();
  }
}
