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
#include "optionvaluelist.h"

#include <utility>

namespace pixpaint
{
  OptionValueList::option_container_type& OptionValueList::getOptions()
  {
    return m_options;
  }

  const OptionValueList::option_container_type& OptionValueList::getOptions() const
  {
    return m_options;
  }

  void OptionValueList::addFlagOption(bool* flag,
                                    std::string name)
  {
    if(canAddOption()) {
      m_options.push_back(OptionValue(std::move(name), flag));
    }
  }

  void OptionValueList::addIntegerValueOption(int* value,
                                              std::string name,
                                              int min,
                                              int max)
  {
    if(canAddOption()) {
      m_options.push_back(OptionValue(std::move(name), value, min, max));
    }
  }

  void OptionValueList::addStringOption(std::string* string, std::string name)
  {
    if(canAddOption()) {
      m_options.push_back(OptionValue(std::move(name), string));
    }
  }

  void OptionValueList::addStringChoice(std::string* string, std::string name, const std::vector<std::string>& choices)
  {
    if(canAddOption()) {
      m_options.push_back(OptionValue(std::move(name), string, choices));
    }
  }

  bool OptionValueList::canAddOption() const
  {
    return m_options.size() < OPTIONS_SIZE_LIMIT;
  }
}
