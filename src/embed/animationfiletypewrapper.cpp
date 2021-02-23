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
#include "animationfiletypewrapper.h"

#include "../image/animation.h"
#include "headerstream.h"
#include "script_errors.h"

namespace pixpaint
{
  void AnimationFileTypeWrapper::onPreSave(const Animation& animation)
  {
    if(auto f = wrapper_base_type::get_override("onPreSave")) {
      try {
        f(boost::cref(animation));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return AnimationFileTypeBase::onPreSave(animation);
  }

  bool AnimationFileTypeWrapper::canReadHeader(HeaderStream& stream) const
  {
    return wrapper_base_type::get_override("canReadHeader")(boost::ref(stream));
  }

  int AnimationFileTypeWrapper::saveAnimation(const std::string& filename, const Animation& animation)
  {
    return wrapper_base_type::get_override("saveImage")(filename, boost::cref(animation));
  }

  int AnimationFileTypeWrapper::openAnimation(const std::string& filename, Animation& animation) const
  {
    return wrapper_base_type::get_override("openImage")(filename, boost::ref(animation));
  }
}
