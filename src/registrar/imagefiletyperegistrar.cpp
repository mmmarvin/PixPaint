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
#include "imagefiletyperegistrar.h"

#include "../image/image.h"

namespace pixpaint
{
  void ImageFileTypeRegistrar::registerFileType(std::string extension,
                                                std::string typeDescription,
                                                std::string id,
                                                std::unique_ptr<ImageFileTypeBase> fileType)
  {
    id = "id_img_" + id;
    if(extension.size() > 12 || typeDescription.size() > 255) {
      // TODO: write error to console here
      return;
    }

    RegistrarBase::registerBuiltIn(std::move(id),
                                   FileInformation<ImageFileTypeHandlerBase>(std::move(extension),
                                                                             std::move(typeDescription),
                                                                             std::make_unique<BuiltInImageFileTypeHandler>(std::move(fileType))));
//    }
  }

  void ImageFileTypeRegistrar::registerCustomFileType(std::string extension,
                                                      std::string typeDescription,
                                                      std::string id,
                                                      boost::python::object fileType)
  {
    id = "id_img_" + id;
    if(extension.size() > 12 || typeDescription.size() > 255) {
      // TODO: write error to console here
      return;
    }

    RegistrarBase::registerCustom(std::move(id),
                                  FileInformation<ImageFileTypeHandlerBase>(std::move(extension),
                                                                            std::move(typeDescription),
                                                                            std::make_unique<CustomImageFileTypeHandler>(fileType)));
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ImageFileTypeRegistrar)
}
