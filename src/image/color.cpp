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
    // formula:
    // s * a + (d * (1 - a))
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

  void multiplyBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // d * s
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2];

    dst[0] = d_r * s_r / 255;
    dst[1] = d_g * s_g / 255;
    dst[2] = d_b * s_b / 255;
  }

  void screenBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // 1 - (1 - d)(1 - s)
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2];

    dst[0] = 255 - (((255 - d_r) * (255 - s_r)) / 255);
    dst[1] = 255 - (((255 - d_g) * (255 - s_g)) / 255);
    dst[2] = 255 - (((255 - d_b) * (255 - s_b)) / 255);
  }

  void overlayBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // 2ds                 if a < 0.5
    // 1 - 2(1 - d)(1 - s) otherwise
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2];

    if(dst[3] < 128) {
      dst[0] = 2 * d_r * s_r / 255;
      dst[1] = 2 * d_g * s_g / 255;
      dst[2] = 2 * d_b * s_b / 255;
    } else {
      dst[0] = 255 - ((2 * (255 - d_r) * (255 - s_r)) / 255);
      dst[1] = 255 - ((2 * (255 - d_g) * (255 - s_g)) / 255);
      dst[2] = 255 - ((2 * (255 - d_b) * (255 - s_b)) / 255);
    }
  }

//  void hardLightBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

//  void softLightBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

  void additionBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // d + s
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2];

    dst[0] = general_utils::min<uint32_t>(255, s_r + d_r);
    dst[1] = general_utils::min<uint32_t>(255, s_g + d_g);
    dst[2] = general_utils::min<uint32_t>(255, s_b + d_b);
  }

  void differenceBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // s - d      if s > d
    // d - s      otherwise
    unsigned int s_r = src[0], s_g = src[1], s_b = src[2];
    unsigned int d_r = dst[0], d_g = dst[1], d_b = dst[2];

    dst[0] = s_r >= d_r ? s_r - d_r : d_r - s_r;
    dst[1] = s_g >= d_g ? s_g - d_g : d_g - s_g;
    dst[2] = s_b >= d_b ? s_b - d_b : d_b - s_b;
  }

  void darkenBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // min(s, d)
    dst[0] = general_utils::min(dst[0], src[0]);
    dst[1] = general_utils::min(dst[1], src[1]);
    dst[2] = general_utils::min(dst[2], src[2]);
  }

  void lightenBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // max(s, d)
    dst[0] = general_utils::max(dst[0], src[0]);
    dst[1] = general_utils::max(dst[1], src[1]);
    dst[2] = general_utils::max(dst[2], src[2]);
  }

  void colorDodgeBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // d / (1 - s)
    dst[0] /= (255 - src[0]);
    dst[1] /= (255 - src[1]);
    dst[2] /= (255 - src[2]);
  }

  void colorBurnBlend(color_channel_t* dst, const color_channel_t* src)
  {
    // formula:
    // (1 - d) / s
    dst[0] = (255 - dst[0]) / src[0];
    dst[1] = (255 - dst[1]) / src[1];
    dst[2] = (255 - dst[2]) / src[2];
  }

//  void srcInBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

//  void dstInBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

//  void srcOutBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

//  void dstOutBlend(color_channel_t* dst, const color_channel_t* src)
//  {

//  }

  void invert(color_channel_t* color)
  {
    color[0] = 255 - color[0];
    color[1] = 255 - color[1];
    color[2] = 255 - color[2];
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
    lhs.r = general_utils::min<uint32_t>(static_cast<uint32_t>(lhs.r) + rhs.r, 255);
    lhs.g = general_utils::min<uint32_t>(static_cast<uint32_t>(lhs.g) + rhs.g, 255);
    lhs.b = general_utils::min<uint32_t>(static_cast<uint32_t>(lhs.b) + rhs.b, 255);
    lhs.a = general_utils::min<uint32_t>(static_cast<uint32_t>(lhs.a) + rhs.a, 255);

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
    lhs.r = general_utils::max<int32_t>(static_cast<int32_t>(lhs.r) - rhs.r, 0);
    lhs.g = general_utils::max<int32_t>(static_cast<int32_t>(lhs.g) - rhs.g, 0);
    lhs.b = general_utils::max<int32_t>(static_cast<int32_t>(lhs.b) - rhs.b, 0);
    lhs.a = general_utils::max<int32_t>(static_cast<int32_t>(lhs.a) - rhs.a, 0);

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
    lhs.r = (static_cast<uint32_t>(lhs.r) * rhs.r) / 255;
    lhs.g = (static_cast<uint32_t>(lhs.g) * rhs.g) / 255;
    lhs.b = (static_cast<uint32_t>(lhs.b) * rhs.b) / 255;
    lhs.a = (static_cast<uint32_t>(lhs.a) * rhs.a) / 255;

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
    color_detail::alphaBlend(reinterpret_cast<unsigned char*>(&ret),
                             reinterpret_cast<const unsigned char*>(&foreground));

    return ret;
  }

  Color invert(const Color& color)
  {
    Color ret = color;
    color_detail::invert(reinterpret_cast<color_channel_t*>(&ret));

    return ret;
  }
}
