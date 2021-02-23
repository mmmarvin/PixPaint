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
#include "default_color_template.h"

#include "image/color.h"

namespace pixpaint
{
  std::vector<Color> getDefaultColor()
  {
    static const std::vector<Color> DEFAULT_COLORS =
    {
      Color::BLACK,
      Color(128, 128, 128, 255),
      Color(128, 0, 0, 255),
      Color(128, 100, 0, 255),
      Color(128, 128, 0, 255),
      Color(0, 128, 0, 255),
      Color(0, 128, 128, 255),
      Color(0, 0, 128, 255),
      Color(128, 0, 128, 255),
      Color::TRANSPARENT,
      Color::WHITE,
      Color(192, 192, 192, 255),
      Color::RED,
      Color(255, 128, 0, 255),
      Color::YELLOW,
      Color::GREEN,
      Color::CYAN,
      Color::BLUE,
      Color(255, 0, 255, 255)
    };

    return DEFAULT_COLORS;
  }
}
