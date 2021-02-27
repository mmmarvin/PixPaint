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
#include "colorpalette.h"

#include <array>
#include <ios>
#include <fstream>
#include <boost/endian.hpp>
#include "assert.h"
#include "default_color_template.h"

namespace pixpaint
{
  static constexpr size_t PALETTE_HEADER_SIZE = 6;
  static const std::array<unsigned char, PALETTE_HEADER_SIZE> PALETTE_HEADER = { 0x1F, 0xFF, 0xBC, 'P', 'P', 'A' };

  ColorPalette::ColorPalette()
  {
    m_colors = getDefaultColor();
  }

  bool ColorPalette::save(const std::string& filename) const
  {
    std::ofstream out(filename, std::ios_base::binary);
    if(out.is_open()) {
      out.write(reinterpret_cast<const char*>(PALETTE_HEADER.data()), PALETTE_HEADER.size());

      boost::endian::little_uint32_t color_count = m_colors.size();
      out.write(reinterpret_cast<char*>(&color_count), sizeof(color_count));

      for(size_t i = 0; i < color_count; ++i) {
        out.write(reinterpret_cast<const char*>(&m_colors[i]), sizeof(Color));
      }

      return true;
    }

    return false;
  }

  bool ColorPalette::load(const std::string& filename)
  {
    std::ifstream in(filename, std::ios_base::binary);
    if(in.is_open()) {
      std::array<unsigned char, PALETTE_HEADER_SIZE> header;
      in.read(reinterpret_cast<char*>(header.data()), header.size());
      if(in.gcount() == header.size()) {
        if(!std::memcmp(PALETTE_HEADER.data(), header.data(), PALETTE_HEADER_SIZE)) {
          boost::endian::little_uint32_t color_count;

          in.read(reinterpret_cast<char*>(&color_count), sizeof(color_count));
          if(in.gcount() == sizeof(color_count) && color_count < MAX_COLOR_COUNT) {
            std::vector<Color> colors;
            colors.resize(color_count);

            for(size_t i = 0; i < color_count; ++i)  {
              in.read(reinterpret_cast<char*>(&colors[i]), sizeof(Color));
              if(in.gcount() != sizeof(Color)) {
                return false;
              }
            }

            m_colors = std::move(colors);
            return true;
          }
        }
      }
    }

    return false;
  }

  void ColorPalette::addColor(const Color& color)
  {
    if(m_colors.size() < MAX_COLOR_COUNT) {
      m_colors.push_back(color);
    }
  }

  void ColorPalette::setColor(size_t index, const Color& color)
  {
    PIXPAINT_ASSERT(index < m_colors.size(), "Index is out-of-bounds of colors");
    m_colors[index] = color;
  }

  void ColorPalette::removeColor(size_t index)
  {
    PIXPAINT_ASSERT(index < m_colors.size(), "Index is out-of-bounds of colors");
    m_colors.erase(m_colors.begin() + index);
  }

  ColorPalette::iterator ColorPalette::begin()
  {
    return m_colors.begin();
  }

  ColorPalette::const_iterator ColorPalette::begin() const
  {
    return m_colors.begin();
  }

  ColorPalette::iterator ColorPalette::end()
  {
    return m_colors.end();
  }

  ColorPalette::const_iterator ColorPalette::end() const
  {
    return m_colors.end();
  }

  size_t ColorPalette::size() const noexcept
  {
    return m_colors.size();
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ColorPalette)
}
