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
#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include "../image/color.h"
#include "../pixpaint_macro.h"

namespace pixpaint
{
  class ColorManager
  {
    ColorManager(const Color& foregroundColor, const Color& background);

  public:
    void setForegroundColor(const Color& color) noexcept;
    void setBackgroundColor(const Color& color) noexcept;

    const Color& getForegroundColor() const noexcept;
    const Color& getBackgroundColor() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(ColorManager)

    Color m_foregroundColor;
    Color m_backgroundColor;
  };

  PIXPAINT_SINGLETON_FUNC(ColorManager)
}

#endif // COLORMANAGER_H
