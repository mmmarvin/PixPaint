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
#include "painttoolregistrar.h"

namespace pixpaint
{
  void PaintToolRegistrar::registerTool(std::string name,
                                        std::string iconFilename,
                                        std::unique_ptr<PaintToolBase> paintTool)
  {
    RegistrarBase::registerBuiltIn(PaintToolInformation(std::move(name),
                                                        std::move(iconFilename),
                                                        std::make_unique<BuiltInPaintToolHandler>(std::move(paintTool))));
  }

  void PaintToolRegistrar::registerCustomTool(std::string name,
                                              std::string iconFilename,
                                              boost::python::object paintToolObject)
  {
    RegistrarBase::registerCustom(PaintToolInformation(std::move(name),
                                                       std::move(iconFilename),
                                                       std::make_unique<CustomPaintToolHandler>(paintToolObject)));
  }

  PIXPAINT_SINGLETON_FUNC_DEF(PaintToolRegistrar)
}
