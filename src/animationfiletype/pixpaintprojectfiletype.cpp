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
#include "pixpaintprojectfiletype.h"

#include <fstream>
#include "../embed/headerstream.h"
#include "../image/animation.h"
#include "../utility/general_utility.h"
#include "../assert.h"
#include "../debug_log.h"

#define CHECK_READ_COUNT(stream, count) \
  if(stream.gcount() != count) { \
    return EOpenResult::EOR_SUCCESS; \
  }

namespace pixpaint
{
namespace
{
  std::string safe_buffer_to_string(const std::array<char, 255>& buff)
  {
    std::string ret;
    for(size_t i = 0; i < 255; ++i) {
      if(buff[i] == '\0')
        return ret;

      ret += buff[i];
    }

    return ret;
  }
}
// PPF Data Structure:
// Header (6 bytes)
// Animation Table
//  Frame 1:
//    Number of Layers  (4 bytes)
//    Frame Duration    (4 bytes)
//  Frame 2:
//    Number of Layers  (4 bytes)
//    Frame Duration    (4 bytes)
//  ...
// Frame Data
//  Layer Name        (255 bytes)
//  Layer Opacity     (4 bytes)
//  Pixel Data...
  const unsigned char PixPaintProjectFileType::HEADER_VALUE[PixPaintProjectFileType::NUMBER_OF_HEADER_BITS] =
  {
    'P', 'P', 'F',
    0x10, 0xF0, 0xD8
  };

  bool PixPaintProjectFileType::canReadHeader(HeaderStream& stream) const
  {
    std::array<unsigned char, NUMBER_OF_HEADER_BITS> header;
    stream.read(reinterpret_cast<char*>(header.data()), header.size());
    if(stream.readCount() == header.size()) {
      return std::memcmp(&header[0], &HEADER_VALUE[0], NUMBER_OF_HEADER_BITS) == 0;
    }

    return false;
  }

  int PixPaintProjectFileType::saveAnimation(const std::string& filename, const Animation& animation)
  {
    if(animation.getFrameCount()) {
      std::ofstream out(filename, std::ios_base::binary);
      if(out.is_open()) {
        ProjectHeader project_header;
        std::memcpy(&project_header.header[0], &HEADER_VALUE[0], sizeof(unsigned char) * NUMBER_OF_HEADER_BITS);
        project_header.num_frames = animation.getFrameCount();
        out.write(reinterpret_cast<char*>(&project_header), sizeof(project_header));

        for(std::size_t i = 0, isize = project_header.num_frames; i < isize; ++i) {
          boost::endian::little_uint32_t num_layers = animation.getFrame(i).getLayerCount();
          boost::endian::little_uint32_t frame_duration = animation.getFrameDuration(i);

          out.write(reinterpret_cast<char*>(&num_layers), sizeof(num_layers));
          out.write(reinterpret_cast<char*>(&frame_duration), sizeof(frame_duration));
        }

        FrameHeader frame_header;
        frame_header.width = animation.getFrame(0).getWidth();
        frame_header.height = animation.getFrame(0).getWidth();
        out.write(reinterpret_cast<char*>(&frame_header), sizeof(frame_header));

        for(std::size_t i = 0, isize = project_header.num_frames; i < isize; ++i) {
          const auto& frame = animation.getFrame(i);
          for(std::size_t j = 0, jsize = frame.getLayerCount(); j < jsize; ++j) {
            // write the layer name
            const auto& layer_name = frame.getLayerName(j);
            std::array<char, 255> layer_name_buff;
            layer_name_buff.fill('\0');
            std::memcpy(layer_name_buff.data(),
                        layer_name.data(),
                        general_utils::min<size_t>(layer_name.size(), 255));
            out.write(layer_name_buff.data(), layer_name_buff.size());

            const auto& layer = frame.getLayer(j);

            // write the layer opacity
            boost::endian::little_uint32_t layer_opacity = layer.getOpacity();
            out.write(reinterpret_cast<char*>(&layer_opacity), sizeof(layer_opacity));

            // write the layer data
            out.write(reinterpret_cast<const char*>(layer.getData()),
                      frame_header.width * frame_header.height * 4 * sizeof(unsigned char));
          }
        }

        return ESaveResult::ESR_SUCCESS;
      }
    } else {
      return ESaveResult::ESR_SUCCESS;
    }

    return ESaveResult::ESR_ERROR;
  }

  int PixPaintProjectFileType::openAnimation(const std::string& filename, Animation& animation) const
  {
    std::ifstream in(filename, std::ios_base::binary);
    if(in.is_open()) {
      ProjectHeader project_header;
      in.read(reinterpret_cast<char*>(&project_header), sizeof(project_header));
      CHECK_READ_COUNT(in, sizeof(project_header))

      if(std::memcmp(&project_header.header[0], &HEADER_VALUE[0], NUMBER_OF_HEADER_BITS) != 0) {
        return EOpenResult::EOR_ERROR;
      }

      std::vector<std::pair<std::size_t, int>> layer_duration_pair;
      layer_duration_pair.resize(project_header.num_frames);
      for(std::size_t i = 0, isize = project_header.num_frames; i < isize; ++i) {
        boost::endian::little_uint32_t number_layers;
        boost::endian::little_uint32_t frame_duration;

        in.read(reinterpret_cast<char*>(&number_layers), sizeof(number_layers));
        CHECK_READ_COUNT(in, sizeof(number_layers))

        in.read(reinterpret_cast<char*>(&frame_duration), sizeof(frame_duration));
        CHECK_READ_COUNT(in, sizeof(frame_duration))

        layer_duration_pair[i] = std::make_pair(number_layers, frame_duration);
      }

      FrameHeader frame_header;
      in.read(reinterpret_cast<char*>(&frame_header), sizeof(frame_header));
      CHECK_READ_COUNT(in, sizeof(frame_header));

      if(frame_header.width < static_cast<int>(PixelData::MINIMUM_DIMENSION) || frame_header.width > static_cast<int>(PixelData::MAXIMUM_DIMENSION) ||
         frame_header.height < static_cast<int>(PixelData::MINIMUM_DIMENSION) || frame_header.height > static_cast<int>(PixelData::MAXIMUM_DIMENSION)) {
        return EOpenResult::EOR_ERROR;
      }

      auto image_size = frame_header.width * frame_header.height * 4 * sizeof(unsigned char);
      animation = Animation();
      for(std::size_t i = 0, isize = project_header.num_frames; i < isize; ++i) {
        auto t = layer_duration_pair[i];
        auto number_layers = std::get<0>(t);
        auto frame_duration = std::get<1>(t);

        if(!number_layers) {
          return EOpenResult::EOR_ERROR;
        }

        Image frame(frame_header.width, frame_header.height);
        frame.removeLayer(0);

        for(std::size_t j = 0; j < number_layers; ++j) {
          frame.addLayer();

          // read the layer name
          std::array<char, 255> buff;
          in.read(buff.data(), buff.size());
          CHECK_READ_COUNT(in, buff.size())
          frame.renameLayer(j, safe_buffer_to_string(buff));

          auto& layer = frame.getLayer(j);

          // read the layer opacity
          boost::endian::little_uint32_t layer_opacity;
          in.read(reinterpret_cast<char*>(&layer_opacity), sizeof(layer_opacity));
          CHECK_READ_COUNT(in, sizeof(layer_opacity))
          layer.setOpacity(layer_opacity);

          // read the layer data
          in.read(reinterpret_cast<char*>(layer.getData()), image_size);
          CHECK_READ_COUNT(in, image_size)
        }

        animation.addFrame(i, std::move(frame), frame_duration);
      }

      return EOpenResult::EOR_SUCCESS;
    }

    return EOpenResult::EOR_ERROR;
  }
}
