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
#include "color.h"

#include <cstring>
#include "../utility/general_utility.h"

namespace pixpaint
{
namespace color_detail
{
  void alphaBlend(color_channel_t* dst, const color_channel_t* src)
  {
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2], s_a = src[3];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2], d_a = dst[3];
    unsigned int s_ia = 255 - s_a;

    if(!s_a) {
      return;
    }

    if(!d_a || !s_ia) {
      std::memcpy(dst, src, 4);
    } else {
      dst[0] = ((s_r * s_a) + (d_r * s_ia)) / 255;
      dst[1] = ((s_g * s_a) + (d_g * s_ia)) / 255;
      dst[2] = ((s_b * s_a) + (d_b * s_ia)) / 255;
      dst[3] = s_a + (d_a * s_ia / 255);
    }
  }
}
  const Color Color::TRANSPARENT = Color(255, 255, 255, 0);
  const Color Color::BLACK = Color(0, 0, 0, 255);
  const Color Color::WHITE = Color(255, 255, 255, 255);
  const Color Color::RED = Color(255, 0, 0, 255);
  const Color Color::GREEN = Color(0, 255, 0, 255);
  const Color Color::BLUE = Color(0, 0, 255, 255);
  const Color Color::YELLOW = Color(255, 255, 0, 255);
  const Color Color::CYAN = Color(0, 255, 255, 255);

  Color::Color() noexcept :
    r(255),
    g(255),
    b(255),
    a(255)
  {
  }

  Color::Color(color_channel_t r_, color_channel_t g_, color_channel_t b_, color_channel_t a_) noexcept :
    r(r_),
    g(g_),
    b(b_),
    a(a_)
  {
  }

  bool operator==(const Color& lhs, const Color& rhs) noexcept
  {
    if(lhs.a == 0 && rhs.a == 0) {
      return true;
    }

    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b &&
           lhs.a == rhs.a;
  }

  bool operator!=(const Color& lhs, const Color& rhs) noexcept
  {
    return !(lhs == rhs);
  }

  Color& operator+=(Color& lhs, const Color& rhs)
  {
    lhs.r = color_channel_t(general_utils::min<int>(static_cast<int>(lhs.r) + rhs.r, 255));
    lhs.g = color_channel_t(general_utils::min<int>(static_cast<int>(lhs.g) + rhs.g, 255));
    lhs.b = color_channel_t(general_utils::min<int>(static_cast<int>(lhs.b) + rhs.b, 255));
    lhs.a = color_channel_t(general_utils::min<int>(static_cast<int>(lhs.a) + rhs.a, 255));

    return lhs;
  }

  Color operator+(const Color& lhs, const Color& rhs)
  {
    auto temp = lhs;
    temp += rhs;
    return temp;
  }

  Color& operator-=(Color& lhs, const Color& rhs)
  {
    lhs.r = color_channel_t(general_utils::max<int>(static_cast<int>(lhs.r) - rhs.r, 0));
    lhs.g = color_channel_t(general_utils::max<int>(static_cast<int>(lhs.g) - rhs.g, 0));
    lhs.b = color_channel_t(general_utils::max<int>(static_cast<int>(lhs.b) - rhs.b, 0));
    lhs.a = color_channel_t(general_utils::max<int>(static_cast<int>(lhs.a) - rhs.a, 0));

    return lhs;
  }

  Color operator-(const Color& lhs, const Color& rhs)
  {
    auto temp = lhs;
    temp += rhs;
    return temp;
  }

  Color& operator*=(Color& lhs, const Color& rhs)
  {
    lhs.r = color_channel_t((static_cast<int>(lhs.r) * rhs.r) / 255);
    lhs.g = color_channel_t((static_cast<int>(lhs.g) * rhs.g) / 255);
    lhs.b = color_channel_t((static_cast<int>(lhs.b) * rhs.b) / 255);
    lhs.a = color_channel_t((static_cast<int>(lhs.a) * rhs.a) / 255);

    return lhs;
  }

  Color operator*(const Color& lhs, const Color& rhs)
  {
    auto temp = lhs;
    temp *= rhs;
    return temp;
  }

  Color alphaBlend(const Color& background, const Color& foreground)
  {
    Color ret = background;
    color_detail::alphaBlend(reinterpret_cast<unsigned char*>(&ret), reinterpret_cast<const unsigned char*>(&foreground));

    return ret;
  }
}
