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
#ifndef ANIMATIONFILETYPEREGISTRAR_H
#define ANIMATIONFILETYPEREGISTRAR_H

#include <string>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../animationfiletype/animationfiletypebase.h"
#include "../pixpaint_macro.h"
#include "information/fileinformation.h"
#include "registrarbase.h"

namespace pixpaint
{
  class AnimationFileTypeRegistrar : public RegistrarBase<FileInformation<AnimationFileTypeHandlerBase>>
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(AnimationFileTypeRegistrar)

  protected:
    AnimationFileTypeRegistrar() {}

  public:
    void registerFileType(std::string exportExtension,
                          std::string exportTypeDescription,
                          std::unique_ptr<AnimationFileTypeBase> exportType);
    void registerCustomFileType(std::string exportExtension,
                                std::string exportTypeDescription,
                                boost::python::object exportType);
  };

  PIXPAINT_SINGLETON_FUNC(AnimationFileTypeRegistrar)
}

#endif // ANIMATIONFILETYPEREGISTRAR_H
