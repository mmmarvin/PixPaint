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
#ifndef IMAGEFILETYPEREGISTRAR_H
#define IMAGEFILETYPEREGISTRAR_H

#include <string>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../filetype/imagefiletypebase.h"
#include "../pixpaint_macro.h"
#include "information/fileinformation.h"
#include "registrarbase.h"

namespace pixpaint
{
  class ImageFileTypeRegistrar : public RegistrarBase<FileInformation<ImageFileTypeHandlerBase>>
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(ImageFileTypeRegistrar)
    ImageFileTypeRegistrar() {}

  public:
    void registerFileType(std::string extension,
                          std::string typeDescription,
                          std::string id,
                          std::unique_ptr<ImageFileTypeBase> fileType);
    void registerCustomFileType(std::string extension,
                                std::string typeDescription,
                                std::string id,
                                boost::python::object fileType);
  };

  PIXPAINT_SINGLETON_FUNC(ImageFileTypeRegistrar)
}

#endif // IMAGEFILETYPEREGISTRAR_H
