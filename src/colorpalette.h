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
#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <string>
#include <vector>
#include "image/color.h"
#include "pixpaint_macro.h"

namespace pixpaint
{
  class ColorPalette
  {
    using container_type = std::vector<Color>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    ColorPalette();

  public:
    static constexpr size_t MAX_COLOR_COUNT = 50;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    void addColor(const Color& color);
    void setColor(size_t index, const Color& color);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    size_t size() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(ColorPalette)

    container_type m_colors;
  };

  PIXPAINT_SINGLETON_FUNC(ColorPalette)
}

#endif // COLORPALETTE_H
