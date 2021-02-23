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
#include "animationfiletypebase.h"

#include "../assert.h"

namespace pixpaint
{
  void AnimationFileTypeBase::onPreSave(const Animation&)
  {
  }

  void AnimationFileTypeHandlerBase::onPreSave(const Animation& animation)
  {
    m_exportTypePtr->onPreSave(animation);
  }

  bool AnimationFileTypeHandlerBase::canReadHeader(HeaderStream& stream) const
  {
    PIXPAINT_ASSERT(m_exportTypePtr, "ExportType pointer was not set");
    return m_exportTypePtr->canReadHeader(stream);
  }

  int AnimationFileTypeHandlerBase::saveAnimation(const std::string& filename, const Animation& animation)
  {
    PIXPAINT_ASSERT(m_exportTypePtr, "ExportType pointer was not set");
    return m_exportTypePtr->saveAnimation(filename, animation);
  }

  int AnimationFileTypeHandlerBase::openAnimation(const std::string& filename, Animation& animation) const
  {
    PIXPAINT_ASSERT(m_exportTypePtr, "ExportType pointer was not set");
    return m_exportTypePtr->openAnimation(filename, animation);
  }

  const AnimationFileTypeBase::option_container_type& AnimationFileTypeHandlerBase::getOptions() const
  {
    PIXPAINT_ASSERT(m_exportTypePtr, "ExportType pointer was not set");
    return m_exportTypePtr->getOptions();
  }

  void AnimationFileTypeHandlerBase::setExportTypeBasePtr(AnimationFileTypeBase* exportType)
  {
    m_exportTypePtr = exportType;
  }

  BuiltInAnimationFileTypeHandlerBase::BuiltInAnimationFileTypeHandlerBase(std::unique_ptr<AnimationFileTypeBase> exportType) :
    m_exportType(std::move(exportType))
  {
    this->setExportTypeBasePtr(m_exportType.get());
  }

  CustomAnimationFileTypeHandlerBase::CustomAnimationFileTypeHandlerBase(boost::python::object exportTypeObject) :
    m_exportTypeObject(exportTypeObject)
  {
    this->setExportTypeBasePtr(boost::python::extract<AnimationFileTypeBase*>(exportTypeObject));
  }
}
