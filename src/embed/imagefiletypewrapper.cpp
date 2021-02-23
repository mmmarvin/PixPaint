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
#include "imagefiletypewrapper.h"

#include "../image/image.h"
#include "headerstream.h"
#include "script_errors.h"

namespace pixpaint
{
  void ImageFileTypeWrapper::onPreSave(const Image& image)
  {
    if(auto f = wrapper_base_type::get_override("onPreSave")) {
      try {
        f(boost::cref(image));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return ImageFileTypeBase::onPreSave(image);
  }

  bool ImageFileTypeWrapper::canReadHeader(HeaderStream& stream) const
  {
    return wrapper_base_type::get_override("canReadHeader")(boost::ref(stream));
  }

  int ImageFileTypeWrapper::saveImage(const std::string& filename, const Image& image)
  {
    return wrapper_base_type::get_override("saveImage")(filename, boost::cref(image));
  }

  int ImageFileTypeWrapper::openImage(const std::string& filename, Image& image) const
  {
    return wrapper_base_type::get_override("openImage")(filename, boost::cref(image));
  }
}
