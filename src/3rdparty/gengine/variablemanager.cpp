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
#include "variablemanager.h"

#include <sstream>
#include "../../assert.h"
#include "converter_utility.h"
#include "string_utility.h"
#include "type_determine_utility.h"

namespace gengine2d
{
  VariableManager::Variable::Variable(value_type&& v) :
    value(std::move(v))
  {
  }

  VariableManager::VariableManager(std::initializer_list<std::tuple<std::string, std::string, EVariableType, bool>> variables)
  {
    for(auto t : variables) {
      auto name = std::get<0>(t);
      auto value = std::get<1>(t);
      auto type = std::get<2>(t);
      auto readOnly = std::get<3>(t);
      PIXPAINT_ASSERT_RESULT(addVariable(std::move(name), std::move(value), type, readOnly));
    }
  }

  bool VariableManager::addVariable(std::string name, std::string value, EVariableType type, bool readOnly)
  {
    switch(type) {
    case EVariableType::Boolean:
      if(type_determine_utils::isBoolean(value)) {
        m_variables.emplace(std::move(name),
                            std::make_tuple(Variable(Variable::value_type(/*std::in_place_type_t<bool>(), */
                                                                          converter_utils::stringTo<bool>(std::move(value)))),
                                            type,
                                            readOnly));
        return true;
      }
      break;
    case EVariableType::Integer:
      if(type_determine_utils::isInteger(value)) {
        m_variables.emplace(std::move(name),
                            std::make_tuple(Variable(Variable::value_type(/*std::in_place_type_t<std::int64_t>(), */
                                                                          converter_utils::stringTo<std::int64_t>(std::move(value)))),
                                            type,
                                            readOnly));
        return true;
      }
      break;
    case EVariableType::FloatingPoint:
      if(type_determine_utils::isFloatingPoint(value)) {
        m_variables.emplace(std::move(name),
                            std::make_tuple(Variable(Variable::value_type(/*std::in_place_type_t<double>(), */
                                                                          converter_utils::stringTo<double>(std::move(value)))),
                                            type,
                                            readOnly));
        return true;
      }
      break;
    case EVariableType::String:
      value = string_utils::tryRemoveQuotations(std::move(value));
      m_variables.emplace(std::move(name),
                          std::make_tuple(Variable(Variable::value_type(/*std::in_place_type_t<std::string>(), */
                                                                        std::move(value))),
                                          type,
                                          readOnly));
      return true;
    default:
      break;
    }

    return false;
  }

  VariableManager::ESetReturn VariableManager::setVariable(const std::string& name, std::string value)
  {
    auto findIt = m_variables.find(name);
    if(findIt != m_variables.end()) {
      if(std::get<2>(findIt->second)) {
        return ESetReturn::IsReadOnly;
      }

      auto type = std::get<1>(findIt->second);
      switch(type) {
      case EVariableType::Boolean:
        if(type_determine_utils::isBoolean(value)) {
          std::get<0>(findIt->second) = Variable::value_type(/*std::in_place_type_t<bool>(),*/
                                                             converter_utils::stringTo<bool>(std::move(value)));
          return ESetReturn::Success;
        } else
          return ESetReturn::InvalidType;
        break;
      case EVariableType::Integer:
        if(type_determine_utils::isInteger(value)) {
          std::get<0>(findIt->second) = Variable::value_type(/*std::in_place_type_t<std::int64_t>(),*/
                                                             converter_utils::stringTo<std::int64_t>(std::move(value)));
          return ESetReturn::Success;
        } else
          return ESetReturn::InvalidType;
        break;
      case EVariableType::FloatingPoint:
        if(type_determine_utils::isFloatingPoint(value)) {
          std::get<0>(findIt->second) = Variable::value_type(/*std::in_place_type_t<double>(),*/
                                                             converter_utils::stringTo<double>(std::move(value)));
          return ESetReturn::Success;
        } else
          return ESetReturn::InvalidType;
        break;
      case EVariableType::String:
        value = string_utils::tryRemoveQuotations(std::move(value));
        std::get<0>(findIt->second) = Variable::value_type(/*std::in_place_type_t<std::string>(),*/
                                                           std::move(value));
        return ESetReturn::Success;
      default:
        break;
      }
    }

    return ESetReturn::InvalidName;
  }

  bool VariableManager::setReadOnly(const std::string& name, bool readOnly)
  {
    auto findIt = m_variables.find(name);
    if(findIt == m_variables.end()) {
      std::get<2>(findIt->second) = readOnly;
      return true;
    }

    return false;
  }

  bool VariableManager::exists(const std::string& name) const
  {
    return m_variables.find(name) != m_variables.end();
  }

  std::size_t VariableManager::size() const
  {
    return m_variables.size();
  }

  std::string VariableManager::getValueString(const std::string& name) const
  {
    auto findIt = m_variables.find(name);
    if(findIt != m_variables.end()) {
      auto type = std::get<1>(findIt->second);
      switch(type) {
      case EVariableType::Boolean:
        return std::to_string(*getBool(name));
      case EVariableType::Integer:
        return std::to_string(*getInteger(name));
      case EVariableType::FloatingPoint:
        return std::to_string(*getFloat(name));
      case EVariableType::String:
        return *getString(name);
      default:
        break;
      }
    }

    return "";
  }

  version_specific::optional<EVariableType> VariableManager::getType(const std::string& name) const
  {
    auto findIt = m_variables.find(name);
    if(findIt != m_variables.end()) {
      return std::get<1>(findIt->second);
    }

    return version_specific::nullopt;
  }

  std::vector<std::string> VariableManager::getVariableList() const
  {
    std::vector<std::string> ret;
    for(const auto& t : m_variables) {
      const auto& key = std::get<0>(t);
//      const auto& value = std::get<1>(t);
      ret.push_back(key);
    }

    return ret;
  }
}
