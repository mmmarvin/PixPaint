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
#include "headerstream.h"

namespace pixpaint
{
  void HeaderStream::read(char* data, std::streamsize size)
  {
    m_stream.read(data, size);
  }

  std::streamsize HeaderStream::readCount() const
  {
    return m_stream.gcount();
  }

  void HeaderStream::reset()
  {
    m_stream.seekg(std::ios_base::beg);
  }
}
