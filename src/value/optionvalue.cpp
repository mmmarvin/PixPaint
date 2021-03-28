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
#include "optionvalue.h"

#include <utility>

namespace pixpaint
{
  OptionValue::OptionValue(std::string optionName,
                           bool* optionValuePtr) :
    m_name(std::move(optionName)),
    m_value(static_cast<void*>(optionValuePtr)),
    m_type(EValueOptionType::FLAG)
  {
  }

  OptionValue::OptionValue(std::string optionName,
                           int* optionValuePtr,
                           int optionFloor,
                           int optionCeiling) :
    m_name(std::move(optionName)),
    m_value(static_cast<void*>(optionValuePtr)),
    m_extraValue(IntegerValue{ optionFloor, optionCeiling }),
    m_type(EValueOptionType::INTEGER_VALUE)
  {
  }

  OptionValue::OptionValue(std::string optionName,
                           std::string* optionValuePtr) :
    m_name(std::move(optionName)),
    m_value(static_cast<void*>(optionValuePtr)),
    m_type(EValueOptionType::STRING_VALUE)
  {
  }

  OptionValue::OptionValue(std::string optionName,
                           std::string* optionValuePtr,
                           const std::vector<std::string>& optionChoices) :
    m_name(std::move(optionName)),
    m_value(static_cast<void*>(optionValuePtr)),
    m_extraValue(StringChoiceValue{ optionChoices }),
    m_type(EValueOptionType::STRING_CHOICE)
  {
  }

  const std::string& OptionValue::getName() const
  {
    return m_name;
  }

  OptionValue::EValueOptionType OptionValue::getType() const
  {
    return m_type;
  }

  void* OptionValue::getValue() const
  {
    return m_value;
  }

  int OptionValue::getFloor() const
  {
    return version_specific::variant_get<IntegerValue>(m_extraValue).floor;
  }

  int OptionValue::getCeil() const
  {
    return version_specific::variant_get<IntegerValue>(m_extraValue).ceil;
  }

  const std::vector<std::string>& OptionValue::getChoices() const
  {
    return version_specific::variant_get<StringChoiceValue>(m_extraValue).values;
  }
}
