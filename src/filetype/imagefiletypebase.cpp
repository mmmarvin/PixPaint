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
#include "imagefiletypebase.h"

#include "../assert.h"

namespace pixpaint
{
  void ImageFileTypeBase::onPreSave(const Image&)
  {
  }

  void ImageFileTypeHandlerBase::onPreSave(const Image& image)
  {
    m_fileTypePtr->onPreSave(image);
  }

  bool ImageFileTypeHandlerBase::canReadHeader(HeaderStream& stream) const
  {
    PIXPAINT_ASSERT(m_fileTypePtr, "FileType pointer was not set");
    return m_fileTypePtr->canReadHeader(stream);
  }

  int ImageFileTypeHandlerBase::saveImage(const std::string& filename, const Image& image)
  {
    PIXPAINT_ASSERT(m_fileTypePtr, "FileType pointer was not set");
    return m_fileTypePtr->saveImage(filename, image);
  }

  int ImageFileTypeHandlerBase::openImage(const std::string& filename, Image& image) const
  {
    PIXPAINT_ASSERT(m_fileTypePtr, "FileType pointer was not set");
    return m_fileTypePtr->openImage(filename, image);
  }

  const ImageFileTypeBase::option_container_type& ImageFileTypeHandlerBase::getOptions() const
  {
    PIXPAINT_ASSERT(m_fileTypePtr, "FileType pointer was not set");
    return m_fileTypePtr->getOptions();
  }

  void ImageFileTypeHandlerBase::setFileTypeBasePtr(ImageFileTypeBase* fileType)
  {
    m_fileTypePtr = fileType;
  }

  BuiltInImageFileTypeHandler::BuiltInImageFileTypeHandler(std::unique_ptr<ImageFileTypeBase> fileType) :
    m_fileType(std::move(fileType))
  {
    this->setFileTypeBasePtr(m_fileType.get());
  }

  CustomImageFileTypeHandler::CustomImageFileTypeHandler(boost::python::object fileTypeObject) :
    m_fileTypeObject(fileTypeObject)
  {
    this->setFileTypeBasePtr(boost::python::extract<ImageFileTypeBase*>(fileTypeObject));
  }
}
