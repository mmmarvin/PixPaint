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
#ifndef ANIMATIONFILETYPEBASE_H
#define ANIMATIONFILETYPEBASE_H

#include <memory>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../value/optionvaluelist.h"

namespace pixpaint
{
  class Animation;
  class Image;
  class HeaderStream;

  class AnimationFileTypeBase : public OptionValueList
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

    AnimationFileTypeBase() {}
    virtual ~AnimationFileTypeBase() {}

    virtual void onPreSave(const Animation& animation);
    virtual bool canReadHeader(HeaderStream& stream) const = 0;
    virtual int saveAnimation(const std::string& filename, const Animation& animation) = 0;
    virtual int openAnimation(const std::string& filename, Animation& animation) const = 0;
  };

  class AnimationFileTypeHandlerBase
  {
  public:
    AnimationFileTypeHandlerBase() {}
    virtual ~AnimationFileTypeHandlerBase() {}

    void onPreSave(const Animation& animation);
    bool canReadHeader(HeaderStream& stream) const;
    int saveAnimation(const std::string& filename, const Animation& animation);
    int openAnimation(const std::string& filename, Animation& animation) const;

    const AnimationFileTypeBase::option_container_type& getOptions() const;

  protected:
    void setExportTypeBasePtr(AnimationFileTypeBase* exportType);

  private:
    AnimationFileTypeBase* m_exportTypePtr;
  };

  class BuiltInAnimationFileTypeHandlerBase : public AnimationFileTypeHandlerBase
  {
  public:
    BuiltInAnimationFileTypeHandlerBase(std::unique_ptr<AnimationFileTypeBase> exportType);

  private:
    std::unique_ptr<AnimationFileTypeBase> m_exportType;
  };

  class CustomAnimationFileTypeHandlerBase : public AnimationFileTypeHandlerBase
  {
  public:
    CustomAnimationFileTypeHandlerBase(boost::python::object exportTypeObject);

  private:
    boost::python::object m_exportTypeObject;
  };
}

#endif // ANIMATIONFILETYPEBASE_H
