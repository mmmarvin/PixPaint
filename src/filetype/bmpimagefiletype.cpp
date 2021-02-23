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
#include "bmpimagefiletype.h"

#include "../3rdparty/stbi/stb_image.h"
#include "../3rdparty/stbi/stb_image_write.h"
#include "../embed/headerstream.h"
#include "../embed/script_utility.h"
#include "../image/image.h"
#include "stbiimageholder.h"

namespace pixpaint
{
  bool BMPImageFileType::canReadHeader(HeaderStream& stream) const
  {
    std::array<unsigned char, 2> header;
    stream.read(reinterpret_cast<char*>(header.data()), header.size());
    if(stream.readCount() == header.size()) {
      return (header[0] == 0x42 &&
              header[1] == 0x4D);
    }

    return false;
  }

  int BMPImageFileType::saveImage(const std::string& filename, const Image& image)
  {
    auto flattened_image = image;
    if(flattened_image.getLayerCount() > 1) {
      flattened_image.flattenLayers(false);
    }

    stbi_write_bmp(filename.c_str(),
                   flattened_image.getWidth(),
                   flattened_image.getHeight(),
                   3,
                   script_utils::convertImageToData(flattened_image, false).data());
    return ESaveResult::ESR_SUCCESS;
  }

  int BMPImageFileType::openImage(const std::string& filename, Image& image) const
  {
    int width, height, n;
    STBIImageDataHolder data(stbi_load(filename.c_str(),
                                       &width,
                                       &height,
                                       &n,
                                       0));
    if(!data.getData()) {
      return EOpenResult::EOR_ERROR;
    }

    image = Image(width, height);
    for(int y = 0; y < height; ++y) {
      for(int x = 0; x < width; ++x) {
        unsigned char* pixel = data.getData() + ((x + y * width) * n);
        image.getLayer(0).setPixel(x,
                                       y,
                                       Color(pixel[0],
                                             pixel[1],
                                             pixel[2],
                                             255));
      }
    }

    return EOpenResult::EOR_SUCCESS;
  }
}
