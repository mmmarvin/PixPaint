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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "variablemanager.h"

namespace gengine2d
{
  class Configuration
  {
  public:
    Configuration() {}

    bool loadFromFile(const std::string& filename);
    bool loadFromStream(std::istream& stream);
    bool saveToFile(const std::string& filename);
    bool saveToStream(std::ostream& stream);

    bool addVariable(const std::string& section,
                     const std::string& name,
                     const std::string& value,
                     EVariableType type);

    int setVariable(const std::string& section, const std::string& name, std::string value);

    const bool* getBool(const std::string& section,
                        const std::string& name) const;
    const std::int64_t* getInteger(const std::string& section,
                                   const std::string& name) const;
    const double* getFloat(const std::string& section,
                           const std::string& name) const;
    const std::string* getString(const std::string& section,
                                 const std::string& name) const;

  private:
    void saveVariables(std::ostream& stream, const VariableManager& variableManager);

    std::unordered_map<std::string, VariableManager>  m_variableSections;
    VariableManager                                   m_universalVariable;
  };

  Configuration& getConfigurationManager();
}

#endif // CONFIGURATION_H
