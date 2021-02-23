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
#include "gifanimationfiletype.h"

#include "../3rdparty/gif-h/gif.h"
#include "../3rdparty/stbi/stb_image.h"
#include "../embed/headerstream.h"
#include "../image/animation.h"
#include "../filetype/stbiimageholder.h"
#include "../utility/filetype_utility.h"
#include "../debug_log.h"

namespace pixpaint
{
  bool GIFAnimationFileType::canReadHeader(HeaderStream& stream) const
  {
    std::array<unsigned char, 6> header;
    stream.read(reinterpret_cast<char*>(header.data()), header.size());
    if(stream.readCount() == header.size()) {
      return (header[0] == 0x47 &&
              header[1] == 0x49 &&
              header[2] == 0x46 &&
              header[3] == 0x38 &&
              header[4] == 0x39 &&
              header[5] == 0x61);
    }

    return false;
  }

  int GIFAnimationFileType::saveAnimation(const std::string& filename, const Animation& animation)
  {
    if(animation.getFrameCount()) {
      // get the delay of the first frame only and use it for the whole animation
      auto delay = animation.getFrameDuration(0) / 10;

      GifWriter writer;
      GifBegin(&writer, filename.c_str(), animation.getFrame(0).getWidth(), animation.getFrame(0).getHeight(), delay);
      for(std::size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
        auto frame = animation.getFrame(i);
        frame.flattenLayers(false);
        GifWriteFrame(&writer, frame.getLayer(0).getData(), frame.getWidth(), frame.getHeight(), delay);
      }
      GifEnd(&writer);

      return ESaveResult::ESR_SUCCESS;
    }

    return ESaveResult::ESR_ERROR;
  }

  int GIFAnimationFileType::openAnimation(const std::string& filename, Animation& animation) const
  {
    auto gif_data = file_utils::readFileToData(filename);
    int* delays;
    if(!gif_data.empty()) {
      int width, height, frame_count, n;
      STBIImageDataHolder data(stbi_load_gif_from_memory(gif_data.data(),
                                                         gif_data.size(),
                                                         &delays,
                                                         &width,
                                                         &height,
                                                         &frame_count,
                                                         &n,
                                                         0));

      if(!data.getData()) {
        return EOpenResult::EOR_ERROR;
      }

      animation = Animation();
      auto image_size = width * height * n;
      for(int i = 0; i < frame_count; ++i) {
        Image frame(width, height);
        for(int y = 0; y < height; ++y) {
          for(int x = 0; x < width; ++x) {
            unsigned char* pixel = data.getData() + (i * image_size) + ((x + y * width) * n);
            frame.getLayer(0).setPixel(x, y, Color(pixel[0], pixel[1], pixel[2], pixel[3]));
          }
        }

        animation.addFrame(i, std::move(frame), delays[i]);
      }

      delete delays;
      return EOpenResult::EOR_SUCCESS;
    }


    return EOpenResult::EOR_ERROR;
  }
}
