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
#include "colormanager.h"

#include "../env/guienvironment.h"
#include "../window/colortoolbox.h"

namespace pixpaint
{
  ColorManager::ColorManager(const Color& foregroundColor, const Color& background) :
    m_foregroundColor(foregroundColor),
    m_backgroundColor(background)
  {
  }

  void ColorManager::setForegroundColor(const Color& color) noexcept
  {
    m_foregroundColor = color;
    getGUIEnvironment().getColorToolbox().setSelectedForegroundColor(color);
  }

  void ColorManager::setBackgroundColor(const Color& color) noexcept
  {
    m_backgroundColor = color;
    getGUIEnvironment().getColorToolbox().setSelectedBackgroundColor(color);
  }

  const Color& ColorManager::getForegroundColor() const noexcept
  {
    return m_foregroundColor;
  }

  const Color& ColorManager::getBackgroundColor() const noexcept
  {
    return m_backgroundColor;
  }

  PIXPAINT_SINGLETON_FUNC_DEFP(ColorManager, Color::BLACK, Color::WHITE)
}
