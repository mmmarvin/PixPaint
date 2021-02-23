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
#ifndef OPTIONVALUE_H
#define OPTIONVALUE_H

#include <string>
#include <vector>
#include "../os_specific_headers.h"

namespace pixpaint
{
  class OptionValue
  {
    struct IntegerValue
    {
      int floor;
      int ceil;
    };

    struct StringChoiceValue
    {
      std::vector<std::string> values;
    };

  public:
    enum class EValueOptionType : unsigned char
    {
      FLAG,
      INTEGER_VALUE,
      STRING_VALUE,
      STRING_CHOICE
    };

    OptionValue(std::string optionName,
                bool* optionValuePtr);
    OptionValue(std::string optionName,
                int* optionValuePtr,
                int optionFloor,
                int optionCeiling);
    OptionValue(std::string optionName,
                std::string* optionValuePtr);
    OptionValue(std::string optionName,
                std::string* optionValuePtr,
                const std::vector<std::string>& optionChoices);

    const std::string& getName() const;
    EValueOptionType getType() const;
    void* getValue() const;
    int getFloor() const;
    int getCeil() const;
    const std::vector<std::string>& getChoices() const;

  private:
    std::string                                           m_name;
    void*                                                 m_value;
    os_specific::variant<IntegerValue, StringChoiceValue> m_extraValue;
    EValueOptionType                                      m_type;
  };
}

#endif // OPTIONVALUE_H
