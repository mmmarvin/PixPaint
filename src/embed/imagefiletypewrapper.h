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
#ifndef IMAGEFILETYPEWRAPPER_H
#define IMAGEFILETYPEWRAPPER_H

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../filetype/imagefiletypebase.h"

namespace pixpaint
{
  class ImageFileTypeWrapper : public ImageFileTypeBase,
                               public boost::python::wrapper<ImageFileTypeBase>
  {
    using wrapper_base_type = boost::python::wrapper<ImageFileTypeBase>;

  public:
    ImageFileTypeWrapper() {}

    void onPreSave(const Image& image) override;
    bool canReadHeader(HeaderStream& stream) const override;
    int saveImage(const std::string& filename, const Image& image) override;
    int openImage(const std::string& filename, Image& image) const override;
  };
}

#endif // IMAGEFILETYPEWRAPPER_H
