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
#ifndef IMAGEFILETYPEBASE_H
#define IMAGEFILETYPEBASE_H

#include <memory>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../value/optionvaluelist.h"

namespace pixpaint
{
  class Image;
  class HeaderStream;

  class ImageFileTypeBase : public OptionValueList
  {
  public:
    enum ESaveResult
    {
      ESR_SUCCESS = 0,
      ESR_NO_SAVE,
      ESR_ERROR
    };

    enum EOpenResult
    {
      EOR_SUCCESS = 0,
      EOR_NO_OPEN,
      EOR_ERROR
    };

    ImageFileTypeBase() {}
    virtual ~ImageFileTypeBase() {}

    virtual void onPreSave(const Image& image);
    virtual bool canReadHeader(HeaderStream& stream) const = 0;
    virtual int saveImage(const std::string& filename, const Image& image) = 0;
    virtual int openImage(const std::string& filename, Image& image) const = 0;
  };

  class ImageFileTypeHandlerBase
  {
  public:
    ImageFileTypeHandlerBase() {}
    virtual ~ImageFileTypeHandlerBase() {}

    void onPreSave(const Image& image);
    bool canReadHeader(HeaderStream& stream) const;
    int saveImage(const std::string& filename, const Image& image);
    int openImage(const std::string& filename, Image& image) const;

    const ImageFileTypeBase::option_container_type& getOptions() const;

  protected:
    void setFileTypeBasePtr(ImageFileTypeBase* fileType);

  private:
    ImageFileTypeBase* m_fileTypePtr;
  };

  class BuiltInImageFileTypeHandler : public ImageFileTypeHandlerBase
  {
  public:
    BuiltInImageFileTypeHandler(std::unique_ptr<ImageFileTypeBase> fileType);

  private:
    std::unique_ptr<ImageFileTypeBase> m_fileType;
  };

  class CustomImageFileTypeHandler : public ImageFileTypeHandlerBase
  {
  public:
    CustomImageFileTypeHandler(boost::python::object fileTypeObject);

  private:
    boost::python::object m_fileTypeObject;
  };
}

#endif // IMAGEFILETYPEBASE_H
