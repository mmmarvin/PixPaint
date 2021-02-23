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
#ifndef OPTIONVALUELIST_H
#define OPTIONVALUELIST_H

#include <string>
#include <vector>
#include "optionvalue.h"

namespace pixpaint
{
  class OptionValueList
  {
    static constexpr auto OPTIONS_SIZE_LIMIT = 50;

  public:
    using option_container_type = std::vector<OptionValue>;

    OptionValueList() {}

    option_container_type& getOptions();
    const option_container_type& getOptions() const;

  protected:
    void addFlagOption(bool* flag, std::string name);
    void addIntegerValueOption(int* value, std::string name, int min = 0, int max = 100);
    void addStringOption(std::string* string, std::string name);
    void addStringChoice(std::string* string, std::string name, const std::vector<std::string>& choices);

  private:
    bool canAddOption() const;

    option_container_type m_options;
  };
}

#endif // OPTIONVALUELIST_H
