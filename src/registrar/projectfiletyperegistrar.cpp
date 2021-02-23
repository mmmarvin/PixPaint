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
#include "projectfiletyperegistrar.h"

namespace pixpaint
{
  void ProjectFileTypeRegistrar::registerFileType(std::string extension,
                                                  std::string typeDescription,
                                                  std::string id,
                                                  std::unique_ptr<AnimationFileTypeBase> exportType)
  {
    id = "id_proj_" + id;
    RegistrarBase::registerBuiltIn(std::move(id),
                                   FileInformation<AnimationFileTypeHandlerBase>(std::move(extension),
                                                                                 std::move(typeDescription),
                                                                                 std::make_unique<BuiltInAnimationFileTypeHandlerBase>(std::move(exportType))));
  }

  void ProjectFileTypeRegistrar::registerCustomFileType(std::string extension,
                                                        std::string typeDescription,
                                                        std::string id,
                                                        boost::python::object exportType)
  {
    id = "id_proj_" + id;
    RegistrarBase::registerCustom(std::move(id),
                                  FileInformation<AnimationFileTypeHandlerBase>(std::move(extension),
                                                                                std::move(typeDescription),
                                                                                std::make_unique<CustomAnimationFileTypeHandlerBase>(exportType)));
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ProjectFileTypeRegistrar)
}
