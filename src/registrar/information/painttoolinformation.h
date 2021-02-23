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
#ifndef PAINTTOOLINFORMATION_H
#define PAINTTOOLINFORMATION_H

#include <memory>
#include "../../tool/painttoolbase.h"

namespace pixpaint
{
  class PaintToolInformation
  {
  public:
    PaintToolInformation(std::string name,
                         std::string iconFilename,
                         std::unique_ptr<PaintToolHandlerBase> paintToolHandler);

    const std::string& getName() const;
    const std::string& getIconFilename() const;
    PaintToolHandlerBase& getTool();
    const PaintToolHandlerBase& getTool() const;

  private:
    std::string                           m_name;
    std::string                           m_iconFilename;
    std::unique_ptr<PaintToolHandlerBase> m_paintToolHandler;
  };
}

#endif // PAINTTOOLINFORMATION_H
