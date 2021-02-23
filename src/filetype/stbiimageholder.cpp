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
#include "stbiimageholder.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rdparty/stbi/stb_image.h"
#include "../3rdparty/stbi/stb_image_write.h"

namespace pixpaint
{
  STBIImageDataHolder::STBIImageDataHolder(unsigned char* data) :
    m_data(data)
  {
  }

  STBIImageDataHolder::~STBIImageDataHolder()
  {
    if(m_data) {
      stbi_image_free(m_data);
    }
  }

  unsigned char* STBIImageDataHolder::getData()
  {
    return m_data;
  }

  const unsigned char* STBIImageDataHolder::getData() const
  {
    return m_data;
  }
}
