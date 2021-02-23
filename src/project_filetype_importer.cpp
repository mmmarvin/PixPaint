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
#include "project_filetype_importer.h"

#include "animationfiletype/pixpaintprojectfiletype.h"
#include "registrar/projectfiletyperegistrar.h"

namespace pixpaint
{
  void register_fixed_project_file_types()
  {
    auto& registrar = getProjectFileTypeRegistrar();
    registrar.registerFileType("ppf", "PixPaint Project File", std::make_unique<PixPaintProjectFileType>());
  }
}
