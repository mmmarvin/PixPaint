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
#include "pixeldata.h"

#include <algorithm>
#include <cstring>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../3rdparty/stbi/stb_image_resize.h"
#include "../utility/general_utility.h"
#include "../utility/math_utility.h"
#include "../utility/qt_utility.h"
#include "../debug_log.h"
#include "pixelmap.h"

namespace pixpaint
{
namespace pixeldata_detail
{
  bool calculate_offset(position_t& x,
                        position_t& y,
                        position_t& xoff,
                        position_t& yoff,
                        std::make_signed_t<dimension_t>& w,
                        std::make_signed_t<dimension_t>& h,
                        std::make_signed_t<dimension_t> layer_width,
                        std::make_signed_t<dimension_t> layer_height)
  {
    if(x + w > layer_width) {
      w = layer_width - x;
    }
    if(y + h > layer_height) {
      h = layer_height - y;
    }

    if(x < 0) {
      w += x;
      xoff = -x;
      x = 0;
    }
    if(y < 0) {
      h += y;
      yoff = -y;
      y = 0;
    }

    if(w < 0 || h < 0) {
      return false;
    }

    w = general_utils::min<std::make_signed_t<dimension_t>>(w, layer_width);
    h = general_utils::min<std::make_signed_t<dimension_t>>(h, layer_height);

    return true;
  }

  bool calculate_offset(position_t& x,
                        position_t& y,
                        std::make_signed_t<dimension_t>& w,
                        std::make_signed_t<dimension_t>& h,
                        std::make_signed_t<dimension_t> layer_width,
                        std::make_signed_t<dimension_t> layer_height)
  {
    if(x + w > layer_width) {
      w = layer_width - x;
    }
    if(y + h > layer_height) {
      h = layer_height - y;
    }

    if(x < 0) {
      w += x;
      x = 0;
    }
    if(y < 0) {
      h += y;
      y = 0;
    }

    if(w < 0 || h < 0) {
      return false;
    }

    w = general_utils::min<std::make_signed_t<dimension_t>>(w, layer_width);
    h = general_utils::min<std::make_signed_t<dimension_t>>(h, layer_height);

    return true;
  }
}
namespace
{
  PixelData fast_scale(const PixelData& layer, dimension_t width, dimension_t height)
  {
    PixelData ret(width, height);

    if(width && height) {
      position_t layer_width = layer.getWidth();
      position_t layer_height = layer.getHeight();
      auto mult_x = float(layer_width) / float(width);
      auto mult_y = float(layer_height) / float(height);
      auto* dst = ret.getData();
      const auto* src = layer.getData();
      auto size = width * 4;

      for(std::size_t j = 0, jsize = width; j < jsize; ++j) {
        auto* dst_ptr = dst;
        for(std::size_t i = 0, isize = height; i < isize; ++i) {
          position_t x = float(i) * mult_x;
          position_t y = float(j) * mult_y;
          if(x < layer_width && y < layer_height) {
            std::memcpy(dst_ptr,
                        src + (static_cast<std::size_t>(y) * 4 * layer_width) + (static_cast<std::size_t>(x) * 4),
                        4);
          }

          dst_ptr += 4;
        }
        dst += size;
      }
    }

    return ret;
  }

  PixelData smooth_scale(PixelData& layer, dimension_t width, dimension_t height)
  {
    PixelData ret(width, height);

    if(width && height) {
      auto* src = layer.getData();
      auto* dst = ret.getData();

      stbir_resize_uint8(src, layer.getWidth(), layer.getHeight(), layer.getWidth() * 4,
                         dst, width, height, width * 4,
                         4);
    }

    return ret;
  }

  PixelData fast_rotate(PixelData& layer, float degree, ERotationDirection direction)
  {
    // rotation formula from https://homepages.inf.ed.ac.uk/rbf/HIPR2/rotate.html
    // x2 = cos(angle) * (x1 - x0) - sin(angle) * (y1 - y0) + x0
    // y2 = sin(angle) * (x1 - x0) + cos(angle) * (y1 - y0) + y0
    std::make_signed_t<dimension_t> layer_width = layer.getWidth();
    std::make_signed_t<dimension_t> layer_height = layer.getHeight();
    if(direction == ERotationDirection::COUNTERCLOCKWISE) {
      degree = 360.f - degree;
    }

    PixelData ret(layer_width, layer_height);

    auto c = center(layer);
    auto rad = math_utils::toRadian(degree);
    double x0 = c.x;
    double y0 = c.y;
    auto cos_r = std::cos(rad);
    auto sin_r = std::sin(rad);

    auto* dst = ret.getData();
    auto* src = layer.getData();
    auto line_width = layer_width * 4;
    for(std::size_t j = 0, jsize = layer_height; j < jsize; ++j) {
      auto* src_ptr = src;
      for(std::size_t i = 0, isize = layer_width; i < isize; ++i) {
        double dx = i - x0;
        double dy = j - y0;
        position_t dst_x = std::round((cos_r * dx) - (sin_r * dy) + x0);
        position_t dst_y = std::round((sin_r * dx) + (cos_r * dy) + y0);
        if(dst_x < 0 || dst_x > layer_width ||
           dst_y < 0 || dst_y > layer_height) {
          src_ptr += 4;
          continue;
        }
        std::memcpy(dst + (dst_y * 4 * layer_width) + (dst_x * 4),
                    src_ptr,
                    4);
        src_ptr += 4;
      }
      src += line_width;
    }

    return ret;
  }
}
  PixelData::PixelData(dimension_t width, dimension_t height, const Color& color) :
    m_data(nullptr),
    m_opacity(100),
    m_width(0),
    m_height(0),
    m_mode(EBlendMode::NORMAL)
  {
    auto size = width * height;
    if(size) {
      std::unique_ptr<Color[]> temp(new Color[size]);
      std::fill_n(temp.get(), size, color);

      m_width = width;
      m_height = height;
      m_data = temp.release();
    }
  }

  PixelData::PixelData(const PixelData& rhs) :
    m_data(nullptr),
    m_opacity(100),
    m_width(0),
    m_height(0),
    m_mode(EBlendMode::NORMAL)
  {
    auto size = rhs.m_width * rhs.m_height;
    if(size) {
      std::unique_ptr<Color[]> temp(new Color[size]);
      std::copy(rhs.m_data, rhs.m_data + size, temp.get());

      m_width = rhs.m_width;
      m_height = rhs.m_height;
      m_opacity = rhs.m_opacity;
      m_mode = rhs.m_mode;
      m_data = temp.release();
    }
  }

  PixelData& PixelData::operator=(const PixelData& rhs)
  {
    PixelData temp(rhs);
    std::swap(temp.m_width, m_width);
    std::swap(temp.m_height, m_height);
    std::swap(temp.m_opacity, m_opacity);
    std::swap(temp.m_mode, m_mode);
    std::swap(temp.m_data, m_data);

    return *this;
  }

  PixelData::PixelData(PixelData&& rhs) noexcept :
    m_data(nullptr),
    m_opacity(100),
    m_width(0),
    m_height(0),
    m_mode(EBlendMode::NORMAL)
  {
    std::swap(rhs.m_width, m_width);
    std::swap(rhs.m_height, m_height);
    std::swap(rhs.m_opacity, m_opacity);
    std::swap(rhs.m_mode, m_mode);
    std::swap(rhs.m_data, m_data);
  }

  PixelData& PixelData::operator=(PixelData&& rhs) noexcept
  {
    std::swap(rhs.m_width, m_width);
    std::swap(rhs.m_height, m_height);
    std::swap(rhs.m_opacity, m_opacity);
    std::swap(rhs.m_mode, m_mode);
    std::swap(rhs.m_data, m_data);

    return *this;
  }

  PixelData::~PixelData()
  {
    if(m_data) {
      delete[] m_data;
    }
  }

  void PixelData::clear(const Color& color)
  {
//    m_pixels.fill(qt_utils::convertToQTColor(color));
    std::fill_n(m_data, m_width * m_height, color);
  }

  void PixelData::clear(position_t x,
                        position_t y,
                        dimension_t width,
                        dimension_t height,
                        const Color& color)
  {
    auto layer_width = static_cast<std::make_signed_t<dimension_t>>(getWidth());
    auto layer_height = static_cast<std::make_signed_t<dimension_t>>(getHeight());
    if(x < layer_width && y < layer_height) {
      std::make_signed_t<dimension_t> w = width;
      std::make_signed_t<dimension_t> h = height;
      if(!pixeldata_detail::calculate_offset(x, y, w, h, layer_width, layer_height)) {
        return;
      }

      auto* dst = reinterpret_cast<Color*>(getData() + (y * 4 * layer_width) + (x * 4));
      dimension_t line_width = layer_width;
      for(std::size_t j = 0; j < h; ++j) {
        auto* dp = dst;
        for(std::size_t i = 0; i < w; ++i) {
          *(dp++) = color;
        }
        dst += line_width;
      }
    }
  }

  void PixelData::resize(dimension_t width,
                         dimension_t height,
                         const Color& color,
                         EResizeMode mode)
  {
    width = general_utils::max<dimension_t>(width, 1);
    height = general_utils::max<dimension_t>(height, 1);

    switch(mode) {
    case EResizeMode::RESIZE:
    {
      PixelData temp(width, height);
      temp.clear(color);
      temp.combine(*this, true);

      std::swap(temp, *this);
    }
      break;
    case EResizeMode::SCALE:
    {
      PixelData temp = fast_scale(*this, width, height);
      std::swap(temp, *this);
    }
      break;
    case EResizeMode::SMOOTH_SCALE:
    {
      PixelData temp = smooth_scale(*this, width, height);
      std::swap(temp, *this);
    }
      break;
    default:
      break;
    }
  }

  dimension_t PixelData::getWidth() const noexcept
  {
    return m_width;
  }

  dimension_t PixelData::getHeight() const noexcept
  {
    return m_height;
  }

  void PixelData::setOpacity(std::uint_least32_t opacity)
  {
    m_opacity = general_utils::min<uint_least32_t>(100, opacity);
  }

  std::uint_least32_t PixelData::getOpacity() const noexcept
  {
    return m_opacity;
  }

  void PixelData::setBlendMode(EBlendMode mode)
  {
    m_mode = mode;
  }

  PixelData::EBlendMode PixelData::getBlendMode() const noexcept
  {
    return m_mode;
  }

  void PixelData::combine(const PixelData& pixelData, bool hard)
  {
    combine(pixelData, 0, 0, hard);
  }

  void PixelData::combine(const PixelMap& pixelMap, bool hard)
  {
    using setter_func_t = std::function<void(position_t, position_t, const Color&)>;
    const auto pixelSetter = hard ?
      setter_func_t([this](position_t x, position_t y, const Color& color) { setPixel(x, y, color); }) :
      setter_func_t([this](position_t x, position_t y, const Color& color) { setPixelSoft(x, y, color); });

    for(const auto& t : pixelMap.m_pixels) {
      const auto& point = std::get<0>(t);
      const auto& color = std::get<1>(t);
      pixelSetter(point.first, point.second, color);
    }
  }

  void PixelData::combine(const PixelData& pixelData, position_t x, position_t y, bool hard)
  {
    combine(pixelData, x, y, hard,
    [](unsigned char* dst_ptr, const unsigned char* src_ptr, position_t, position_t) {
      std::memcpy(dst_ptr, src_ptr, 4);
    },
    [](unsigned char* dst_ptr, const unsigned char* src_ptr, position_t, position_t) {
      color_detail::alphaBlend(dst_ptr, src_ptr);
    });
  }

  void PixelData::composite(const PixelData& pixelData, position_t x, position_t y)
  {
    composite(pixelData, x, y, [](auto*, auto*, auto, auto) { return true; });
  }

  PixelData PixelData::copy(position_t x, position_t y, dimension_t width, dimension_t height) const
  {
    PixelData ret(width, height, Color::TRANSPARENT);

    auto layer_width = static_cast<std::make_signed_t<dimension_t>>(getWidth());
    auto layer_height = static_cast<std::make_signed_t<dimension_t>>(getHeight());
    if(x < layer_width && y < layer_height) {
      std::make_signed_t<dimension_t> w = width;
      std::make_signed_t<dimension_t> h = height;
      position_t dstXOff = 0, dstYOff = 0;
      if(!pixeldata_detail::calculate_offset(x, y, dstXOff, dstYOff, w, h, layer_width, layer_height)) {
        return ret;
      }

      auto* src = getData() + (y * 4 * layer_width) + (x * 4);
      auto* dst = ret.getData() + (dstYOff * 4 * ret.getWidth()) + (dstXOff * 4);
      auto src_width = layer_width * 4;
      auto dst_width = ret.getWidth() * 4;
      auto size = w * 4;
      for(std::size_t j = 0; j < h; ++j) {
        std::memcpy(dst, src, size);
        src += src_width;
        dst += dst_width;
      }
    }

    return ret;
  }

  PixelData PixelData::cut(position_t x, position_t y, dimension_t width, dimension_t height)
  {
    PixelData ret(width, height, Color::TRANSPARENT);

    auto layer_width = static_cast<std::make_signed_t<dimension_t>>(getWidth());
    auto layer_height = static_cast<std::make_signed_t<dimension_t>>(getHeight());
    if(x < layer_width && y < layer_height) {
      std::make_signed_t<dimension_t> w = width;
      std::make_signed_t<dimension_t> h = height;
      position_t dstXOff = 0, dstYOff = 0;
      if(!pixeldata_detail::calculate_offset(x, y, dstXOff, dstYOff, w, h, layer_width, layer_height)) {
        return ret;
      }

      auto* src = getData() + (y * 4 * layer_width) + (x * 4);
      auto* dst = ret.getData() + (dstYOff * 4 * ret.getWidth()) + (dstXOff * 4);
      auto src_width = layer_width * 4;
      auto dst_width = ret.getWidth() * 4;
      for(std::size_t j = 0; j < h; ++j) {
        auto* src_ptr = src;
        auto* dst_ptr = dst;
        for(std::size_t i = 0; i < w; ++i) {
          std::memcpy(dst_ptr, src_ptr, 4);
          src_ptr[0] = 0;
          src_ptr[1] = 0;
          src_ptr[2] = 0;
          src_ptr[3] = 0;
          src_ptr += 4;
          dst_ptr += 4;
        }
        src += src_width;
        dst += dst_width;
      }
    }

    return ret;
  }

  void PixelData::setPixelSoft(position_t x, position_t y, const Color& color)
  {
    if(x < 0 || y < 0 ||
       x > getWidth() || y > getHeight()) {
      PIXPAINT_DEBUG_LOG("Cannot set PixelData because position [", x, y, "] is outside the boundaries");
      return;
    }

    auto* data = getData() + (y * 4 * getWidth()) + (x * 4);
    color_detail::alphaBlend(data, reinterpret_cast<const unsigned char*>(&color));
  }

  void PixelData::setPixel(position_t x, position_t y, const Color& color)
  {
    if(x < 0 || y < 0 ||
       x > getWidth() || y > getHeight()) {
      PIXPAINT_DEBUG_LOG("Cannot set PixelData because position [", x, y, "] is outside the boundaries");
      return;
    }

    *reinterpret_cast<Color*>(getData() + (y * 4 * getWidth()) + (x * 4)) = color;
  }

  Color PixelData::getPixel(position_t x, position_t y) const
  {
    if(x < 0 || y < 0 ||
       x > getWidth() || y > getHeight()) {
      PIXPAINT_DEBUG_LOG("Cannot get PixelData because position [", x, y, "] is outside the boundaries");
      return Color::TRANSPARENT;
    }

    return *(m_data + (y * getWidth()) + x);
  }

  void PixelData::setPixelRowSoft(position_t row, const Color* color, std::size_t size)
  {
    setPixelRowSoft(row, 0, color, size);
  }

  void PixelData::setPixelRow(position_t row, const Color* color, std::size_t size)
  {
    setPixelRow(row, 0, color, size);
  }

  void PixelData::setPixelRowSoft(position_t row, position_t index, const Color* color, std::size_t size)
  {
    auto* data = getData() + (row * 4 * getWidth()) + (index * 4);
    for(std::size_t i = 0; i < size; ++i) {
      color_detail::alphaBlend(data, reinterpret_cast<const unsigned char*>(&color[i]));
      data += 4;
    }
  }

  void PixelData::setPixelRow(position_t row, position_t index, const Color* color, std::size_t size)
  {
    auto* data = getData() + (row * 4 * getWidth()) + (index * 4);
    std::memcpy(data, color, sizeof(Color) * size);
  }

  const Color* PixelData::getPixelRow(position_t row) const
  {
    return reinterpret_cast<const Color*>(getData() + (row * 4 * getWidth()));
  }

  void PixelData::rotate(float degree, ERotationDirection direction)
  {
    if(degree < 0.f) {
      if(direction == ERotationDirection::CLOCKWISE) {
        direction = ERotationDirection::COUNTERCLOCKWISE;
      } else {
        direction = ERotationDirection::CLOCKWISE;
      }
      degree = -degree;
    }
    degree = fmod(degree, 360.f);

    PixelData temp = fast_rotate(*this, degree, direction);
    std::swap(temp, *this);
  }

  PixelData PixelData::flipHorizontally() const
  {
    PixelData ret(getWidth(), getHeight());

    if(getWidth() > 0 && getHeight() > 0) {
      auto* src = getData();
      auto* dst = ret.getData();
      auto line_width = getWidth();
      auto byte_line_width = line_width * 4;

      for(std::size_t j = 0, jsize = getHeight(); j < jsize; ++j) {
        auto* sp = src + byte_line_width - 4;
        auto* dp = dst;
        for(std::size_t i = 0; i < line_width; ++i) {
          dp[0] = sp[0];
          dp[1] = sp[1];
          dp[2] = sp[2];
          dp[3] = sp[3];
          sp -= 4;
          dp += 4;
        }

        src += byte_line_width;
        dst += byte_line_width;
      }
    }

    return ret;
  }

  PixelData PixelData::flipVertically() const
  {
    PixelData ret(getWidth(), getHeight());

    if(getWidth() > 0 && getHeight() > 0) {
      auto* src1 = getData();
      auto* src2 = getData() + ((getHeight() - 1) * 4 * getWidth()) ;
      auto* dst1 = ret.getData() + ((ret.getHeight() - 1) * 4 * ret.getWidth()) ;
      auto* dst2 = ret.getData();
      auto size = getWidth() * 4;

      while(src1 < src2) {
        std::memcpy(dst1, src1, size);
        std::memcpy(dst2, src2, size);
        src1 += size;
        src2 -= size;
        dst1 -= size;
        dst2 += size;
      }
    }

    return ret;
  }

  void PixelData::invert()
  {
    auto size = getWidth() * getHeight();
    auto* data = getData();

    for(std::size_t i = 0; i < size; ++i) {
      data[0] = general_utils::max<int>(255 - data[0], 0);
      data[1] = general_utils::max<int>(255 - data[1], 0);
      data[2] = general_utils::max<int>(255 - data[2], 0);
      data += 4;
    }
  }

  PixelData PixelData::toFitted() const
  {
    auto* c = reinterpret_cast<const Color*>(getData());

    Point min, max;
    position_t w = m_width, h = m_height;
    for(position_t y = 0; y < h; ++y) {
      auto* c_ptr = c;
      for(position_t x = 0; x < w; ++x) {
        if(c_ptr->a > 0) {
          if(x < min.x) {
            min.x = x;
          } else if(x > max.x) {
            max.x = x;
          }
          if(y < min.y) {
            min.y = y;
          } else if(y > max.y) {
            max.y = y;
          }
        }
        ++c_ptr;
      }
      c += m_width;
    }

    auto dim = max - min;
    return copy(min.x, min.y, dim.x, dim.y);
  }

  color_channel_t* PixelData::getData() noexcept
  {
    return reinterpret_cast<color_channel_t*>(m_data);
  }

  const color_channel_t* PixelData::getData() const noexcept
  {
    return reinterpret_cast<const color_channel_t*>(m_data);
  }

  Point center(const PixelData& layer)
  {
    return Point(layer.getWidth() / 2, layer.getHeight() / 2);
  }
}
