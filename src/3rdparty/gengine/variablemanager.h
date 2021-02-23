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
#ifndef VARIABLEMANAGER_H
#define VARIABLEMANAGER_H

#include <cassert>
#include <unordered_map>
#include <vector>
#include "../../os_specific_headers.h"

namespace gengine2d
{
  enum class EVariableType : unsigned char
  {
    Boolean,
    Integer,
    FloatingPoint,
    String
  };

  class VariableManager
  {
    struct Variable
    {
      using value_type = os_specific::variant<bool, std::int64_t, double, std::string>;

      Variable(value_type&& v);

      value_type value;
    };

  public:
    enum class ESetReturn : unsigned char
    {
      Success = 0,
      InvalidType,
      InvalidName,
      IsReadOnly
    };

    explicit VariableManager(std::initializer_list<std::tuple<std::string, std::string, EVariableType, bool>> variables = {});

    bool addVariable(std::string name, std::string value, EVariableType type, bool readOnly = false);
    ESetReturn setVariable(const std::string& name, std::string value);
    bool setReadOnly(const std::string& name, bool readOnly);

    bool exists(const std::string& name) const;
    std::size_t size() const;

    const bool* getBool(const std::string& name) const { return getVariable<bool>(name); }
    const std::int64_t* getInteger(const std::string& name) const { return getVariable<std::int64_t>(name); }
    const double* getFloat(const std::string& name) const { return getVariable<double>(name); }
    const std::string* getString(const std::string& name) const { return getVariable<std::string>(name); }
    std::string getValueString(const std::string& name) const;
    os_specific::optional<EVariableType> getType(const std::string& name) const;

    std::vector<std::string> getVariableList() const;

  private:
    template<class T> const T* getVariable(const std::string& name) const;

    // TODO: Add a function callback to when a value is set
    // ex. when renderer values such as window.width / window.height
    // is changed, the callback function will update the renderer window size
    // on set
    std::unordered_map<std::string, std::tuple<Variable, EVariableType, bool>> m_variables;
  };

  template<class T>
  const T* VariableManager::getVariable(const std::string& name) const
  {
    auto findIt = m_variables.find(name);
    assert(findIt != m_variables.end());

    if(findIt != m_variables.end()) {
      const T* ret = os_specific::variant_get_if<T>(&std::get<0>(findIt->second).value);
      assert(ret);

      return ret;
    }

    return nullptr;
  }
}

#endif // VARIABLEMANAGER_H
