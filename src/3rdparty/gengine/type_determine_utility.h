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
#ifndef TYPE_DETERMINE_UTILITY_H
#define TYPE_DETERMINE_UTILITY_H

#include <string>

namespace gengine2d
{
namespace type_determine_utils
{
  bool isBoolean(const std::string& str);
  bool isInteger(const std::string& str);
  bool isFloatingPoint(const std::string& str);
}
}

#endif // TYPE_DETERMINE_UTILITY_H
