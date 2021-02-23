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
#include "configuration.h"

#include <algorithm>
#include <fstream>
#include "../../debug_log.h"
#include "string_utility.h"

namespace gengine2d
{
  bool Configuration::loadFromFile(const std::string& filename)
  {
    std::ifstream in(filename);
    if(in.is_open()) {
      return loadFromStream(in);
    }

    return false;
  }

  bool Configuration::loadFromStream(std::istream& stream)
  {
    std::string currentSection = "";
    std::string line;

    while(std::getline(stream, line)) {
      auto tokens = string_utils::tokenizeIf(line, [](char c) { return isspace(c) || c == '='; });
      if(tokens.size() == 1) {
        auto token = tokens.back();
        if(token.size() > 2 && token.front() == '[' && token.back() == ']') {
          currentSection = token.substr(1, token.size() - 2);
        } else {
          PIXPAINT_DEBUG_LOG("Unknown \"" + token + "\"");
        }
      } else if(tokens.size() == 2) {
        auto name = tokens[0];
        auto value = tokens[1];

        auto setResult = setVariable(currentSection, name, value);
        if(setResult == -1) {
          PIXPAINT_DEBUG_LOG("Unknown section \"" + currentSection + "\"");
        } else {
          switch(static_cast<VariableManager::ESetReturn>(setResult)) {
          case VariableManager::ESetReturn::Success:
            break;
          case VariableManager::ESetReturn::InvalidType:
            PIXPAINT_DEBUG_LOG("\"" + name + "\" is of invalid type");
            break;
          case VariableManager::ESetReturn::InvalidName:
            PIXPAINT_DEBUG_LOG("Invalid name");
            break;
          case VariableManager::ESetReturn::IsReadOnly:
            PIXPAINT_DEBUG_LOG("\"" + name + "\" is read only");
            break;
          default:
            break;
          }
        }
      }
    }

    return true;
  }

  bool Configuration::saveToFile(const std::string& filename)
  {
    std::ofstream out(filename);
    if(out.is_open()) {
      return saveToStream(out);
    }
    return false;
  }

  bool Configuration::saveToStream(std::ostream& stream)
  {
    if(m_universalVariable.size()) {
      stream << "[Universal]" << std::endl;
      saveVariables(stream, m_universalVariable);
      stream << std::endl;
    }

    for(const auto& t : m_variableSections) {
      const auto& section = std::get<0>(t);
      const auto& variableList = std::get<1>(t);
      stream << "[" << section << "]" << std::endl;
      saveVariables(stream, variableList);
      stream << std::endl;
    }

    return true;
  }

  bool Configuration::addVariable(const std::string& section,
                                  const std::string& name,
                                  const std::string& value,
                                  EVariableType type)
  {
    if(section.empty() || section == "Universal") {
      return m_universalVariable.addVariable(name, value, type);
    }

    auto findIt = m_variableSections.find(section);
    if(findIt == m_variableSections.end()) {
      auto res = m_variableSections.emplace(section, VariableManager());
      if(res.second) {
        return res.first->second.addVariable(name, value, type);
      } else {
        return false;
      }
    }

    return findIt->second.addVariable(name, value, type);
  }

  int Configuration::setVariable(const std::string& section, const std::string& name, std::string value)
  {
    if(section.empty() || section == "Universal") {
      return static_cast<int>(m_universalVariable.setVariable(name, std::move(value)));
    }

    auto findIt = m_variableSections.find(section);
    if(findIt != m_variableSections.end()) {
      return static_cast<int>(findIt->second.setVariable(name, std::move(value)));
    }

    return -1;
  }

  const bool* Configuration::getBool(const std::string& section,
                                     const std::string& name) const
  {
    if(section.empty() || section == "Universal") {
      return m_universalVariable.getBool(name);
    }

    auto findIt = m_variableSections.find(section);
    if(findIt != m_variableSections.end()) {
      return findIt->second.getBool(name);
    }

    return nullptr;
  }

  const std::int64_t* Configuration::getInteger(const std::string& section,
                                                const std::string& name) const
  {
    if(section.empty() || section == "Universal") {
      return m_universalVariable.getInteger(name);
    }

    auto findIt = m_variableSections.find(section);
    if(findIt != m_variableSections.end()) {
      return findIt->second.getInteger(name);
    }

    return nullptr;
  }

  const double* Configuration::getFloat(const std::string& section,
                                        const std::string& name) const
  {
    if(section.empty() || section == "Universal") {
      return m_universalVariable.getFloat(name);
    }

    auto findIt = m_variableSections.find(section);
    if(findIt != m_variableSections.end()) {
      return findIt->second.getFloat(name);
    }

    return nullptr;
  }

  const std::string* Configuration::getString(const std::string& section,
                                           const std::string& name) const
  {
    if(section.empty() || section == "Universal") {
      return m_universalVariable.getString(name);
    }

    auto findIt = m_variableSections.find(section);
    if(findIt != m_variableSections.end()) {
      return findIt->second.getString(name);
    }

    return nullptr;
  }

  void Configuration::saveVariables(std::ostream& stream, const VariableManager& variableManager)
  {
    auto variableList = variableManager.getVariableList();
    std::sort(variableList.begin(), variableList.end());
    for(const auto& variableName : variableList) {
      auto variableType = variableManager.getType(variableName).value();
      switch(variableType) {
      case EVariableType::Boolean:
        stream << variableName << "=" << *variableManager.getBool(variableName) ? std::string("true") :
                                                                                  std::string("false");
        stream << std::endl;
        break;
      case EVariableType::String:
      {
        auto value = variableManager.getValueString(variableName);
        if(value == "\"\"") {
          stream << variableName << "=" << value << "";
        } else {
          stream << variableName << "=\"" << value << "\"";
        }
        stream << std::endl;
      }
        break;
      default:
        stream << variableName << "=" << variableManager.getValueString(variableName);
        stream << std::endl;
        break;
      }
    }
  }

  Configuration& getConfigurationManager()
  {
    static Configuration instance;
    return instance;
  }
}
