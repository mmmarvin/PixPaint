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
#include "painttoolinformation.h"

#include "../../assert.h"

namespace pixpaint
{
  PaintToolInformation::PaintToolInformation(std::string name,
                                             std::string iconFilename,
                                             std::unique_ptr<PaintToolHandlerBase> paintToolHandler) :
    m_name(std::move(name)),
    m_iconFilename(std::move(iconFilename)),
    m_paintToolHandler(std::move(paintToolHandler))
  {
    PIXPAINT_ASSERT(m_paintToolHandler.get(), "Empty effect");
  }

  const std::string& PaintToolInformation::getName() const
  {
    return m_name;
  }

  const std::string& PaintToolInformation::getIconFilename() const
  {
    return m_iconFilename;
  }

  PaintToolHandlerBase& PaintToolInformation::getTool()
  {
    return *m_paintToolHandler.get();
  }

  const PaintToolHandlerBase& PaintToolInformation::getTool() const
  {
    return *m_paintToolHandler.get();
  }
}
