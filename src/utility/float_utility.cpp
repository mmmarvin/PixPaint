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
#include "float_utility.h"

#include <limits>
#include "general_utility.h"

namespace pixpaint
{
namespace float_utils
{
  bool isZero(float f)
  {
    return f < std::numeric_limits<float>::epsilon() && f > -std::numeric_limits<float>::epsilon();
  }

  bool isEqual(float a, float b)
  {
    return general_utils::fastAbs(b - a) <= std::numeric_limits<float>::epsilon();
  }
}
}
