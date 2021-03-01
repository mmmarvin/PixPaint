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
#ifndef PIXELDATA_H
#define PIXELDATA_H

#include <cstddef>
#include <type_traits>
#include "../type.h"
#include "color.h"
#include "point.h"

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
                        std::make_signed_t<dimension_t> layer_height);
  bool calculate_offset(position_t& x,
                        position_t& y,
                        std::make_signed_t<dimension_t>& w,
                        std::make_signed_t<dimension_t>& h,
                        std::make_signed_t<dimension_t> layer_width,
                        std::make_signed_t<dimension_t> layer_height);
}
  class PixelMap;

  enum class ERotationDirection : unsigned char
  {
    CLOCKWISE = 0,
    COUNTERCLOCKWISE
  };

  class PixelData
  {
  public:
    static constexpr std::size_t MINIMUM_DIMENSION = 1;
    static constexpr std::size_t MAXIMUM_DIMENSION = 100'000;

    enum class EResizeMode : unsigned char
    {
      RESIZE = 0,
      SCALE,
      SMOOTH_SCALE
    };

    PixelData(dimension_t width, dimension_t height, const Color& color = Color::WHITE);
    PixelData(const PixelData& rhs);
    PixelData& operator=(const PixelData& rhs);
    PixelData(PixelData&& rhs) noexcept;
    PixelData& operator=(PixelData&& rhs) noexcept;
    ~PixelData();

    void clear(const Color& color = Color::WHITE);
    void clear(position_t x,
               position_t y,
               dimension_t width,
               dimension_t height,
               const Color& color = Color::WHITE);

    void resize(dimension_t width,
                dimension_t height,
                const Color& color = Color::WHITE,
                EResizeMode mode = EResizeMode::RESIZE);
    dimension_t getWidth() const noexcept;
    dimension_t getHeight() const noexcept;

    void setOpacity(std::uint_least32_t opacity);
    std::uint_least32_t getOpacity() const noexcept;

    void combine(const PixelData& pixelData, bool hard = false);
    void combine(const PixelMap& pixelMap, bool hard = false);
    void combine(const PixelData& pixelData, position_t x, position_t y, bool hard = false);
    PixelData copy(position_t x, position_t y, dimension_t width, dimension_t height) const;
    PixelData cut(position_t x, position_t y, dimension_t width, dimension_t height);

    void setPixelSoft(position_t x, position_t y, const Color& color);
    void setPixel(position_t x, position_t y, const Color& color);
    Color getPixel(position_t x, position_t y) const;

    void setPixelRowSoft(position_t row, const Color* color, std::size_t size);
    void setPixelRow(position_t row, const Color* color, std::size_t size);
    void setPixelRowSoft(position_t row, position_t index, const Color* color, std::size_t size);
    void setPixelRow(position_t row, position_t index, const Color* color, std::size_t size);
    const Color* getPixelRow(position_t row) const;

    void rotate(float degree, ERotationDirection direction);
    PixelData flipHorizontally() const;
    PixelData flipVertically() const;
    void invert();

    PixelData toFitted() const;

    color_channel_t* getData() noexcept;
    const color_channel_t* getData() const noexcept;

  protected:
    template<class HardFunc, class Func>
    void combine(const PixelData& pixelData, position_t x, position_t y, bool hard, HardFunc hardFunc, Func func);

  private:
    Color*              m_data;
    std::uint_least32_t m_opacity;
    dimension_t         m_width;
    dimension_t         m_height;
  };

  Point center(const PixelData& layer);
}

#include "pixeldata.inl"

#endif // PIXELDATA_H
