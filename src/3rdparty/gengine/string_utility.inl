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
#include "string_utility.h"

namespace gengine2d
{
namespace string_utils
{
  template<class Func>
  std::vector<std::string> tokenizeIf(std::string str, Func&& func)
  {
    std::vector<std::string> ret;

    std::size_t i = 0;
    while(i < str.size()) {
      while(i < str.size() && std::forward<Func>(func)(str[i])) {
        ++i;
      }

      std::size_t j = i;
      while(j < str.size() && !std::forward<Func>(func)(str[j])) {
        ++j;
      }

      if(i < j) {
        ret.push_back(str.substr(i, j - i));;
        i = j;
      }
    }

    return ret;
  }
}
}
