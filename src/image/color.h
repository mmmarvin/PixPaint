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
#ifndef COLOR_H
#define COLOR_H

#include "../type.h"

namespace pixpaint
{
namespace color_detail
{
  void alphaBlend(color_channel_t* dst, const color_channel_t* src);
  void multiplyBlend(color_channel_t* dst, const color_channel_t* src);
  void screenBlend(color_channel_t* dst, const color_channel_t* src);
  void overlayBlend(color_channel_t* dst, const color_channel_t* src);
  void additionBlend(color_channel_t* dst, const color_channel_t* src);
  void differenceBlend(color_channel_t* dst, const color_channel_t* src);
}
#pragma pack(push, 1)
  struct Color
  {
    static const Color TRANSPARENT;
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color CYAN;

    Color() noexcept;
    Color(color_channel_t r_, color_channel_t g_, color_channel_t b_, color_channel_t a_ = 255) noexcept;

    color_channel_t r;
    color_channel_t g;
    color_channel_t b;
    color_channel_t a;
  };
#pragma pack(pop)

  bool operator==(const Color& lhs, const Color& rhs) noexcept;
  bool operator!=(const Color& lhs, const Color& rhs) noexcept;
  Color& operator+=(Color& lhs, const Color& rhs);
  Color operator+(const Color& lhs, const Color& rhs);
  Color& operator-=(Color& lhs, const Color& rhs);
  Color operator-(const Color& lhs, const Color& rhs);
  Color& operator*=(Color& lhs, const Color& rhs);
  Color operator*(const Color& lhs, const Color& rhs);

  Color alphaBlend(const Color& background, const Color& foreground);
}

#endif // COLOR_H
