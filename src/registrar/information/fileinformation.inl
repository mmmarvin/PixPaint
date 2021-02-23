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
#include "fileinformation.h"

namespace pixpaint
{
  template<class HandlerBaseType>
  FileInformation<HandlerBaseType>::FileInformation(std::string extension,
                                   std::string description,
                                   std::unique_ptr<HandlerBaseType> type) :
    m_extension(std::move(extension)),
    m_description(std::move(description)),
    m_type(std::move(type))
  {
  }

  template<class HandlerBaseType>
  const std::string& FileInformation<HandlerBaseType>::getExtension() const
  {
    return m_extension;
  }

  template<class HandlerBaseType>
  const std::string& FileInformation<HandlerBaseType>::getDescription() const
  {
    return m_description;
  }

  template<class HandlerBaseType>
  HandlerBaseType& FileInformation<HandlerBaseType>::getType()
  {
    return *m_type.get();
  }

  template<class HandlerBaseType>
  const HandlerBaseType& FileInformation<HandlerBaseType>::getType() const
  {
    return *m_type.get();
  }
}
