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
#include "type_determine_utility.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include "string_utility.h"

namespace gengine2d
{
namespace type_determine_utils
{
namespace
{
  bool is_integer_impl(const std::string& str)
  {
    bool neg = false;
    for(auto c : str) {
      if(!isdigit(c)) {
        if(c == '-') {
          if(!neg) {
            neg = true;
          } else {
            return false;
          }
        }
      }
    }

    return true;
  }

  bool is_floating_point_impl(const std::string& str)
  {
    bool neg = false, point = false;
    for(auto c : str) {
      if(!isdigit(c)) {
        if(c == '-') {
          if(!neg) {
            neg = true;
          } else {
            return false;
          }
        } else if(c == '.') {
          if(!point) {
            point = true;
          } else {
            return false;
          }
        }
      }
    }

    return true;
  }
}
  bool isBoolean(const std::string& str)
  {
    return string_utils::toLower(str) == "true" || string_utils::toLower(str) == "false";
  }

  bool isInteger(const std::string& str)
  {
    static const auto MAX_INTEGER_STR = std::to_string(std::numeric_limits<int>::max());
    static const auto MIN_INTEGER_STR = std::to_string(std::numeric_limits<int>::min());

    if(std::find(str.begin(), str.end(), '-') != str.end() &&
       str.size() <= MIN_INTEGER_STR.size()) {
      return is_integer_impl(str);
    } else  if(str.size() <= MAX_INTEGER_STR.size()) {
      return is_integer_impl(str);
    }

    return false;
  }

  bool isFloatingPoint(const std::string& str)
  {
    if(str.size() <= 7) {
      return is_floating_point_impl(str);
    }

    if(str.size() == 8 &&
       (std::find(str.begin(), str.end(), '-') != str.end() ||
        std::find(str.begin(), str.end(), '.') != str.end())) {
      return is_floating_point_impl(str);
    }

    if(str.size() == 9 &&
       std::find(str.begin(), str.end(), '-') != str.end() &&
        std::find(str.begin(), str.end(), '.') != str.end()) {
      return is_floating_point_impl(str);
    }

    return false;
  }
}
}
