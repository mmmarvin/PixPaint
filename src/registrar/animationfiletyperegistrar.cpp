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
#include "animationfiletyperegistrar.h"

namespace pixpaint
{
  void AnimationFileTypeRegistrar::registerFileType(std::string exportExtension,
                                                    std::string exportTypeDescription,
                                                    std::unique_ptr<AnimationFileTypeBase> exportType)
  {
//    if(!exportExtension.empty()) {
      RegistrarBase::registerBuiltIn(FileInformation<AnimationFileTypeHandlerBase>(std::move(exportExtension),
                                                                                   std::move(exportTypeDescription),
                                                                                   std::make_unique<BuiltInAnimationFileTypeHandlerBase>(std::move(exportType))));
//    }
  }

  void AnimationFileTypeRegistrar::registerCustomFileType(std::string exportExtension,
                                                          std::string exportTypeDescription,
                                                          boost::python::object exportType)
  {
//    if(!exportExtension.empty()) {
      RegistrarBase::registerCustom(FileInformation<AnimationFileTypeHandlerBase>(std::move(exportExtension),
                                                                                  std::move(exportTypeDescription),
                                                                                  std::make_unique<CustomAnimationFileTypeHandlerBase>(exportType)));
//    }
  }

  PIXPAINT_SINGLETON_FUNC_DEF(AnimationFileTypeRegistrar)
}
